#include "qCalSteppingAction.hh"
#include "qCalEventAction.hh"
#include "qCalTrackingAction.hh"
#include "qCalTrajectory.hh"
#include "qCalPMTSD.hh"
#include "qCalUserTrackInformation.hh"
#include "qCalUserEventInformation.hh"
#include "qCalSteppingMessenger.hh"
#include "qCalRecorderBase.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"


qCalSteppingAction::qCalSteppingAction()
:fOneStepPrimaries(false)
{
   fSteppingMessenger = new qCalSteppingMessenger(this);
   
   fExpectedNextStatus = Undefined;
}


qCalSteppingAction::~qCalSteppingAction()
{
   
}


void qCalSteppingAction::UserSteppingAction(const G4Step * theStep)
{
   
   G4Track* theTrack = theStep->GetTrack();
   
   if ( theTrack->GetCurrentStepNumber() == 1 ) fExpectedNextStatus = Undefined;
   
   qCalUserTrackInformation* trackInformation = (qCalUserTrackInformation*)theTrack->GetUserInformation();
   qCalUserEventInformation* eventInformation = (qCalUserEventInformation*)G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();
   
   G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
   G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();
   
   G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
   G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
   
   G4OpBoundaryProcessStatus boundaryStatus=Undefined;
   static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;
   
   //find the boundary process only once
   if ( !boundary )
   {
      G4ProcessManager* pm = theStep->GetTrack()->GetDefinition()->GetProcessManager();
      G4int nprocesses     = pm->GetProcessListLength();
      G4ProcessVector* pv  = pm->GetProcessList();
      G4int i;
      for ( i = 0; i < nprocesses; i++ )
      {
         if ( (*pv)[i]->GetProcessName() == "OpBoundary" )
         {
            boundary = (G4OpBoundaryProcess*)(*pv)[i];
            break;
         }
      }
   }
   
   if ( theTrack->GetParentID() == 0 )
   {
      //This is a primary track
      
      G4TrackVector* fSecondary  = fpSteppingManager->GetfSecondary();
      G4int tN2ndariesTot        = fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
      
      //If we havent already found the conversion position and there were
      //secondaries generated, then search for it
      if ( !eventInformation->IsConvPosSet() && tN2ndariesTot > 0 )
      {
         for ( size_t lp1 = (*fSecondary).size() - tN2ndariesTot; lp1<(*fSecondary).size(); lp1++ )
         {
            const G4VProcess* creator=(*fSecondary)[lp1]->GetCreatorProcess();
            if ( creator )
            {
               G4String creatorName=creator->GetProcessName();
               if ( creatorName == "phot" || creatorName == "compt" || creatorName == "conv" )
               {
                  //since this is happening before the secondary is being tracked
                  //the Vertex position has not been set yet(set in initial step)
                  eventInformation->SetConvPos((*fSecondary)[lp1]->GetPosition());
               }
            }
         }
      }
      
      if( fOneStepPrimaries&&thePrePV->GetName() == "scintillator" )
      {
         theTrack->SetTrackStatus(fStopAndKill);
      }
   }
   
   if (!thePostPV ) //out of world
   {
      fExpectedNextStatus = Undefined;
      return;
   }
   
   G4ParticleDefinition* particleType = theTrack->GetDefinition();
   if ( particleType == G4OpticalPhoton::OpticalPhotonDefinition() )
   {
      // Optical photon only
      
      if ( thePrePV->GetName() == "Quartz")
      {
         // force drawing of photons in WLS slab
         trackInformation->SetForceDrawTrajectory(true);
      }
      
      // Was the photon absorbed by the absorption process
      if ( thePostPoint->GetProcessDefinedStep()->GetProcessName() == "OpAbsorption")
      {
         eventInformation->IncAbsorption();
         trackInformation->AddTrackStatusFlag(absorbed);
      }
      
      boundaryStatus = boundary->GetStatus();
      
      //Check to see if the partcile was actually at a boundary
      //Otherwise the boundary status may not be valid
      //Prior to Geant4.6.0-p1 this would not have been enough to check
      if ( thePostPoint->GetStepStatus() == fGeomBoundary )
      {
         if ( fExpectedNextStatus == StepTooSmall )
         {
            if ( boundaryStatus != StepTooSmall )
            {
               G4ExceptionDescription ed;
               ed << "qCalSteppingAction::UserSteppingAction(): "
               << "No reallocation step after reflection!"
               << G4endl;
               G4Exception("qCalSteppingAction::UserSteppingAction()", "qCalExpl01",
                           FatalException,ed,
                           "Something is wrong with the surface normal or geometry");
            }
         }
         
         fExpectedNextStatus=Undefined;
         
         switch ( boundaryStatus )
         {
            case Absorption:
               trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
               eventInformation->IncBoundaryAbsorption();
               break;
            case Detection: //Note, this assumes that the volume causing detection
               //is the photocathode because it is the only one with
               //non-zero efficiency
            {
               //Triger sensitive detector manually since photon is
               //absorbed but status was Detection
               G4SDManager* SDman = G4SDManager::GetSDMpointer();
               G4String sdName="/qCalDet/SiPMSD";
               qCalSiPMSD* SiPMSD = (qCalSiPMSD*)SDman->FindSensitiveDetector(sdName);
               if(SiPMSD)SiPMSD->ProcessHits_constStep(theStep,NULL);
               trackInformation->AddTrackStatusFlag(hitSiPM);
               break;
            }
            case FresnelReflection:
            case TotalInternalReflection:
            case LambertianReflection:
            case LobeReflection:
            case SpikeReflection:
            case BackScattering:
               trackInformation->IncReflections();
               fExpectedNextStatus=StepTooSmall;
               break;
            default:
               break;
         }
      }
   }
}
