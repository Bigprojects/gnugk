//////////////////////////////////////////////////////////////////
//
// BroadcastListen.h thread for listening to broadcasts (only needed on some OSs)
//
// This work is published under the GNU Public License (GPL)
// see file COPYING for details.
//
// History:
// 	991016	initial version (Jan Willamowius)
// 	991020	code cleanup (Jan Willamowius)
//
//////////////////////////////////////////////////////////////////


#ifndef BROADCASTLISTEN_H
#define BROADCASTLISTEN_H

#include "ptlib.h"
#include "ptlib/sockets.h"
#include "h225.h" 

class H323RasSrv;

class BroadcastListen : public PThread
{
	  PCLASSINFO(BroadcastListen, PThread)
public:
	BroadcastListen(H323RasSrv * _RasSrv);
	virtual ~BroadcastListen();

	void Close(void);

protected:
	virtual void Main(void);

protected:
	PUDPSocket BroadcastListener;
	H323RasSrv * RasSrv;
};

#endif // BROADCASTLISTEN_H

