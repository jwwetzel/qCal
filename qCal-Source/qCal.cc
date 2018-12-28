/**********************************
 Quartz Based Cherenkov Calorimeter
 Produced by Dr. James Wetzel
 Contact: james-wetzel@uiowa.edu
 The University of Iowa
 July - 2018
 **********************************/

//#include "qCalRecorderBase.hh"
#include "qCalDetectorConstruction.hh"
#include "qCalActionInitialization.hh"
#include "qCalPhysicsList.hh"

#include <cstdlib>
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4String.hh"
#include "G4VUserDetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4NistManager.hh"

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
      G4cerr << " qCal [-m macro ] [-w cubeWidth] [-x nQuartzXAxis] [-y nQuartzYAxis] [-z nQuartzZAxis] [-a absorberZ] [-u UIsession] [-t nThreads]" << G4endl;
      G4cerr << "   note: -t option is available only for multi-threaded mode." << G4endl;
      G4cerr << "Example: ./qCal -m runMe.Mac -w 1.0 -x 10 -y 10 -z 50 -a Fe -u \"qt\" -t 4" << G4endl;
   }
}

int main(int argc, char** argv)
{
   //Make sure there aren't too many args
   if( argc > 15 )
   {
      G4cout << " Too many Arguments " << G4endl;
      PrintUsage();
      return 1;
   }
   
   //For storing the macro name argument
   G4String macro;
   
   //For the session argument
   G4String session;
   
   //For the number of Quartz cubes in each access from arguments, default is 1x1x1
   G4int nXAxis = 1;
   G4int nYAxis = 1;
   G4int nZAxis = 1;
   
   G4float nCubeWidth = 1.0; // 1.0 cm cube by default
   
   //Absorber Z
   G4String sAbs = "Fe";
   
   //If multithreaded option, instantiate nThreads
   #ifdef G4MULTITHREADED
   G4int nThreads = 0;
   #endif
   
   //Loop through the arguments and check to make sure they are in the correct order.
   G4cout << "If you receive a segmentation fault here, you probably entered the wrong arguments." << G4endl;
   for ( G4int i=1; i<argc; i=i+2 )
   {
      if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
      else if ( G4String(argv[i]) == "-w" ) nCubeWidth = atof(argv[i+1]);
      else if ( G4String(argv[i]) == "-x" ) nXAxis = atoi(argv[i+1]);
      else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
      else if ( G4String(argv[i]) == "-y" ) nYAxis = atoi(argv[i+1]);
      else if ( G4String(argv[i]) == "-z" ) nZAxis = atoi(argv[i+1]);
      else if ( G4String(argv[i]) == "-a" ) sAbs  = argv[i+1];
   #ifdef G4MULTITHREADED
      else if ( G4String(argv[i]) == "-t" ) {
         nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
      }
   #endif
      else {
         G4cout << "Incorrect Usage, arguments read are: " << G4endl;
         G4cout << "nXAxis: " << nXAxis << G4endl;
         G4cout << "nYAxis: " << nYAxis << G4endl;
         G4cout << "nZAxis: " << nZAxis << G4endl;
         G4cout << "sAbs: " << sAbs << G4endl;
         G4cout << "nThreads: " << nThreads << G4endl;
         G4cout << "Macro: " << macro << G4endl;
         G4cout << "Session: " << session << G4endl;
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
   
   
   /**************************************************************************************
    User initialization classes must be set to G4RunManager through SetUserInitialization
    **************************************************************************************/
   //Initialize Detector Construction
   G4NistManager* nist = G4NistManager::Instance();
   G4Material* absorberMat = nist->FindOrBuildMaterial("G4_"+sAbs);
   G4float fAbsRadLen = absorberMat->GetRadlen()*mm;
   auto detConstruction = new qCalDetectorConstruction(nXAxis, nYAxis, nZAxis, sAbs, fAbsRadLen, nCubeWidth);
   runManager->SetUserInitialization(detConstruction);
   
   //Configure Physics List for initialization
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
   
   //Initialize Physics List
   runManager->SetUserInitialization(physicsList);
   
   //Initialize Actions, includes primary generator
   auto actionInitialization = new qCalActionInitialization();
   runManager->SetUserInitialization(actionInitialization);
   
   runManager->Initialize();
   
   
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
   
   return 0;
}












































































































































