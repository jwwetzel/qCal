#include "G4VUserEventInformation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#ifndef qCalUserEventInformation_h
#define qCalUserEventInformation_h 1

class qCalUserEventInformation : public G4VUserEventInformation
{
public:
   
   qCalUserEventInformation();
   virtual ~qCalUserEventInformation();
   
   inline virtual void Print()const{};
   
   void IncPhotonCount_Ceren(){fPhotonCount_Ceren++;}
   void IncEDep(G4double dep){fTotE+=dep;}
   void IncAbsorption(){fAbsorptionCount++;}
   void IncBoundaryAbsorption(){fBoundaryAbsorptionCount++;}
   void IncHitCount(G4int i=1){fHitCount+=i;}
   
   void SetEWeightPos(const G4ThreeVector& p){fEWeightPos=p;}
   void SetReconPos(const G4ThreeVector& p){fReconPos=p;}
   void SetConvPos(const G4ThreeVector& p){fConvPos=p;fConvPosSet=true;}
   void SetPosMax(const G4ThreeVector& p,G4double edep){fPosMax=p;fEdepMax=edep;}
   
   G4int GetPhotonCount_Ceren()const {return fPhotonCount_Ceren;}
   G4int GetHitCount()const {return fHitCount;}
   G4double GetEDep()const {return fTotE;}
   G4int GetAbsorptionCount()const {return fAbsorptionCount;}
   G4int GetBoundaryAbsorptionCount() const {return fBoundaryAbsorptionCount;}
   
   G4ThreeVector GetEWeightPos(){return fEWeightPos;}
   G4ThreeVector GetReconPos(){return fReconPos;}
   G4ThreeVector GetConvPos(){return fConvPos;}
   G4ThreeVector GetPosMax(){return fPosMax;}
   G4double GetEDepMax(){return fEdepMax;}
   G4double IsConvPosSet(){return fConvPosSet;}
   
   //Gets the total photon count produced
   G4int GetPhotonCount(){return fPhotonCount_Ceren;}
   
   void IncSiPMSAboveThreshold(){fSiPMsAboveThreshold++;}
   G4int GetSiPMSAboveThreshold(){return fSiPMsAboveThreshold;}
   
private:
   
   G4int fHitCount;
   G4int fPhotonCount_Ceren;
   G4int fAbsorptionCount;
   G4int fBoundaryAbsorptionCount;
   
   G4double fTotE;
   
   //These only have meaning if totE > 0
   //If totE = 0 then these wont be set by EndOfEventAction
   G4ThreeVector fEWeightPos;
   G4ThreeVector fReconPos; //Also relies on hitCount>0
   G4ThreeVector fConvPos;//true (initial) converstion position
   G4bool fConvPosSet;
   G4ThreeVector fPosMax;
   G4double fEdepMax;
   
   G4int fSiPMsAboveThreshold;
   
};

#endif

