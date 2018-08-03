#include "qCalUserTrackInformation.hh"


qCalUserTrackInformation::qCalUserTrackInformation()
: fStatus(active), fReflections(0), fForcedraw(false)
{
   
}


qCalUserTrackInformation::~qCalUserTrackInformation()
{
   
}


void qCalUserTrackInformation::AddTrackStatusFlag( int s )
{
//   if ( s&active ) //track is now active
//   {
//      fStatus&=~inactive; //remove any flags indicating it is inactive
//   }
//   else if ( s&inactive ) //track is now inactive
//   {
//      fStatus&=~active; //remove any flags indicating it is active
//   }
//   fStatus|=s; //add new flags
}

