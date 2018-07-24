#ifndef qCalPhysicsList_h
#define qCalPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class qCalPhysicsList: public G4VModularPhysicsList
{
public:
   
   qCalPhysicsList();
   virtual ~qCalPhysicsList();
   
   // SetCuts()
   virtual void SetCuts();
   
};
#endif
