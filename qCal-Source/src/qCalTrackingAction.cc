#include "qCalTrajectory.hh"
#include "qCalTrackingAction.hh"
#include "qCalUserTrackInformation.hh"
#include "qCalDetectorConstruction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"


qCalTrackingAction::qCalTrackingAction()
{
   
}


void qCalTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
   //Use custom trajectory class
   fpTrackingManager->SetTrajectory(new qCalTrajectory(aTrack));

//   //This user track information is only relevant to the photons
   fpTrackingManager->SetUserTrackInformation(new qCalUserTrackInformation);
}



void qCalTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
   qCalTrajectory* trajectory = (qCalTrajectory*)fpTrackingManager->GimmeTrajectory();
   qCalUserTrackInformation* trackInformation = (qCalUserTrackInformation*)aTrack->GetUserInformation();
//
//   //Lets choose to draw only the photons that hit a SiPM
   if ( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() )
   {
      const G4VProcess* creator = aTrack->GetCreatorProcess();

      if( trackInformation->GetTrackStatus() & hitSiPM )
      {
         trajectory->SetDrawTrajectory(true);
      }
   }
   else //draw all other trajectories
   {
      trajectory->SetDrawTrajectory(false);
   }
//   if(trackInformation->GetForceDrawTrajectory())
//   {
//      trajectory->SetDrawTrajectory(true);
//   }
}
