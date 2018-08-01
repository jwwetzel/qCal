#include "qCalHit.hh"

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <iomanip>

G4ThreadLocal G4Allocator<qCalHit>* qCalSiPMHitAllocator=0;

qCalHit::qCalHit()
:fPmtNumber(-1),fPhotons(0),fPhysVol(0),fDrawit(false)
{
   
}

qCalHit::~qCalHit()
{
   
}

void qCalHit::Draw()
{
   if(fDrawit&&fPhysVol){ //ReDraw only the PMTs that have hit counts > 0
      //Also need a physical volume to be able to draw anything
      G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
      if(pVVisManager){//Make sure that the VisManager exists
         G4VisAttributes attribs(G4Colour(1.,0.,0.));
         attribs.SetForceSolid(true);
         G4RotationMatrix rot;
         if(fPhysVol->GetRotation())//If a rotation is defined use it
            rot=*(fPhysVol->GetRotation());
         G4Transform3D trans(rot,fPhysVol->GetTranslation());//Create transform
         pVVisManager->Draw(*fPhysVol,attribs,trans);//Draw it
      }
   }
}

void qCalHit::Print()
{
   
}
