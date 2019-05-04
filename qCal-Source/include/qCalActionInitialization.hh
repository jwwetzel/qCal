#ifndef qCalActionInitialization_h
#define qCalActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "qCalDetectorConstruction.hh"

class qCalDetectorConstruction;
//class qCalRecorderBase;

class qCalActionInitialization : public G4VUserActionInitialization
{
public:
    qCalActionInitialization();
    virtual ~qCalActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
   qCalDetectorConstruction* p_theDetector;
   G4double p_gunXLocation;
   G4double p_gunYLocation;
   G4double p_gunZLocation;
};
#endif

