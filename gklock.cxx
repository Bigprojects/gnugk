// -*- mode: c++; eval: (c-set-style "linux"); -*-
//////////////////////////////////////////////////////////////////
//
// Classes for locking
//
// Copyright (c) Nils Bokermann <nils.bokermann@mediaways.net>
//
// This work is published under the GNU Public License (GPL)
// see file COPYING for details.
// We also explicitely grant the right to link this code
// with the OpenH323 library.
//
//
//////////////////////////////////////////////////////////////////

#include <ptlib.h>
#include "Toolkit.h"
#include "gklock.h"


#ifndef lint
// mark object with version info in such a way that it is retrievable by
// the std. version/revision control tools like RCS/CVS ident cmd. At
// least the strings cmd will extract this info.
static const char gkid[] = GKGVS;
static const char vcid[] = "@(#) $Id$";
static const char vcHid[] = GKLOCK_H;
#endif /* lint */


// GKCondMutex
void
GKCondMutex::Lock()
{
	Wait();
	access_count +=1;
	Signal();
}

void
GKCondMutex::Unlock()
{
	Wait();
 	if(access_count >0) {
		access_count -=1;
	}
//	PTRACE(5, "GKCondMutex: " << access_count);
	Signal();
}

BOOL
GKCondMutex::Condition()
{
//	PTRACE(5, "access_count is: " << access_count);
	return access_count==0;
}

// ProxyCondMutex

void
ProxyCondMutex::Lock(const PString & name)
{
	Wait();
	access_count +=1;
	PString myname=name;
	int i=0;
	while(locker.GetStringsIndex(myname)!=P_MAX_INDEX)
		myname=name + PString(++i);
	locker.AppendString(myname);
	PTRACE(5, "ProxyCondMutex::Lock(this=" << this << ") " << access_count << " from " << name );
	Signal();
}

void
ProxyCondMutex::Unlock(const PString &name)
{
	Wait();
	PTRACE(5, "ProxyCondMutex::Unlock(this=" << this << ") " << access_count << " lockers: " << locker);
	PAssert(access_count>0, "unlocking unlocked.");
 	if(access_count >0) {
		PTRACE(5, "deleting Lock of:" << name << " with place " << locker.GetStringsIndex(name));
		int i=0;
		while(locker.GetStringsIndex(name+PString(++i))!=P_MAX_INDEX)
			;
		if(0==--i) {
			PTRACE(5, "Name: " << name << " StringsIndex: " << locker.GetStringsIndex(name));
			if (locker.GetStringsIndex(name)!=P_MAX_INDEX) {
				access_count -=1;
				locker.RemoveAt(locker.GetStringsIndex(name));
			} else
				PTRACE(1, "removing non-locked");
		} else {
			access_count -= 1;
			locker.RemoveAt(locker.GetStringsIndex(name + PString(i)));
		}
	}
	PAssert(locker.GetSize()==access_count, "unlocking wrong!");
	Signal();
}

BOOL
ProxyCondMutex::Condition()
{
	PTRACE(5, "ProxyCondMutex::Condition(this=" << this <<")" << "access_count is: " << access_count << "locks of: " << locker);
	return access_count==0;
}

void
ProxyCondMutex::OnWait()
{
	for(PINDEX i; i<locker.GetSize(); i++)
		PTRACE(5,"locker[" << i << "]: " << locker[i]);
}

void ProxyCondMutex::WaitCondition()
{
	PTRACE(5, "ProxyCondMutex::WaitCondition()");
	PCondMutex::WaitCondition();
}
