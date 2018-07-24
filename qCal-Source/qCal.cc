#include "SipmRecorderBase.hh"
#include "SipmDetectorConstruction.hh"
#include "SipmActionInitialization.hh"
#include "SipmPhysicsList.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4String.hh"
#include "G4VUserDetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

namespace {
   void PrintUsage() {
      G4cerr << " Usage: " << G4endl;
      G4cerr << " qCal [-m macro ] [-x nQuartzXAxis] [-y nQuartzYAxis] [-z nQuartzZAxis] [-a absorberZ] [-u UIsession] [-t nThreads]" << G4endl;
      G4cerr << "   note: -t option is available only for multi-threaded mode."
      G4cerr << "Example: ./qCal -m runMe.Mac -x 10 -Y 10 -Z 50 -a 26 -u notSure -t 4"
      << G4endl;
   }
}

int main(int argc, char** argv)
{
   //Make sure there aren't too many args
   if( argc > 13 )
   {
      PrintUsage();
      return 1;
   }
   
   //For storing the macro name argument
   G4String macro;
   
   //For the session argument
   G4String session;
   
   //For the number of Quartz cubes in each access from arguments, default is 1x1x1
   G4Int nXAxis = 1;
   G4Int nYAxis = 1;
   G4Int nZAxis = 1;
   
   //Absorber Z
   G4Int
   
   //If multithreaded option, instantiate nThreads
   #ifdef G4MULTITHREADED
   G4int nThreads = 0;
   #endif
   
   //Loop through the arguments
   for ( G4int i=1; i<argc; i=i+2 ) {
      if ( G4String(argv[i]) == "-x" ) nXAxis = argv[i+1];
      if ( G4String(argv[i]) == "-y" ) nYAxis = argv[i+1];
      if ( G4String(argv[i]) == "-z" ) nZAxis = argv[i+1];
      if ( G4String(argv[i]) == "-a" ) nAbsZ  = argv[i+1];
      if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
      else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
      #ifdef G4MULTITHREADED
      else if ( G4String(argv[i]) == "-t" ) {
         nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
      }
      #endif
      else {
         PrintUsage();
         return 1;
      }
   }
   
   //Detect interactive mode (no macro provided), define UI session
   G4UIExecutive* ui = nullptr;
   if (! macro.size())
   {
      ui = new G4UIExecutive(argc, argv, session);
   }
   
   //Construct the default Run Manager
   #ifdef G4MULTITHREADED
   auto runManager = new G4MTRunManager;
   if ( nThreads > 0 ) {
      runManager->SetNumberOfThreads(nThreads);
   }
   #else
   auto runManager = new G4RunManager;
   #endif
   
   //Initialize Detector Construction
   auto detConstruction = new SipmDetectorConstruction(nXAxis,nYAxis,nZAxis);
   runManager->SetUserInitialization(detConstruction);
   
   
   //Initialize Physics List
   auto physicsList = new FTFP_BERT;
   physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
   
   G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
   opticalPhysics->SetWLSTimeProfile("delta");
   opticalPhysics->SetScintillationYieldFactor(1.0);
   opticalPhysics->SetScintillationExcitationRatio(0.0);
   opticalPhysics->SetMaxNumPhotonsPerStep(100);
   opticalPhysics->SetMaxBetaChangePerStep(10.0);
   opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true);
   opticalPhysics->SetTrackSecondariesFirst(kScintillation, true);
   
   physicsList->RegisterPhysics(opticalPhysics);
   
   runManager->SetUserInitialization(physicsList);
   runManager->SetUserInitialization(new SipmActionInitialization());
   
   //Initialize Actions
   auto actionInitialization = new SipmActionInitialization(detConstruction);
   runManager->SetUserInitialization(actionInitialization);
   
   
   
   //Initialize Visualization
   // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
   // G4VisManager* visManager = new G4VisExecutive("Quiet");
   auto visManager = new G4VisExecutive;
   visManager->Initialize();
   
   //Get the pointer to the User Interface Manager
   auto UImanager = G4UImanager::GetUIpointer();
   
   //Process the macro or start the UI Session
   
   if ( macro.size() )
   {
      //batch mode
      G4String command = "/control/execute ";
      UImanager->ApplyCommand(command+macro);
   }
   else
   {
      // interactive mode : define UI session
      UImanager->ApplyCommand("/control/execute init_vis.mac");
      if (ui->IsGUI()) {
         UImanager->ApplyCommand("/control/execute gui.mac");
      }
      ui->SessionStart();
      delete ui;
   }
   
   // Job termination
   // Free the store: user actions, physics_list and detector_description are
   // owned and deleted by the run manager, so they should not be deleted
   // in the main() program !
   
   delete visManager;
   delete runManager;
   
}












































































































































