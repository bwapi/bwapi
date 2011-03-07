
#include "CriticalSection.h"

// constructors

CriticalSection::CriticalSection()
{
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&anchor);
}

void CriticalSection::init()
{
  InitializeCriticalSection(&anchor);
}


CriticalSection::Lock::Lock() : target(NULL)
{
}

CriticalSection::Lock::Lock(CriticalSection& fresh_target) : target(NULL)
{
	lock(fresh_target);
}

void CriticalSection::Lock::lock(CriticalSection& fresh_target)
{
	if(target)
		release();
	target = &fresh_target;
	EnterCriticalSection(&(target->anchor));
}

bool CriticalSection::Lock::tryLock(CriticalSection& fresh_target)
{
	bool locked = !!TryEnterCriticalSection(&(fresh_target.anchor));
	if(locked)
		target = &fresh_target;
	else
		target = NULL;
	return locked;
}

CriticalSection::Lock::~Lock()
{
	release();
}

void CriticalSection::Lock::release()
{
	if(!target)
		return;
	LeaveCriticalSection(&(target->anchor));
	target = NULL;
}
