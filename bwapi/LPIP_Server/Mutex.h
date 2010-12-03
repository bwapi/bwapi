#ifndef __MUTEX_H__
#define __MUTEX_H__
#pragma once

class Mutex
{
public:
	Mutex(LPCWSTR name)
	{
		this->m_hMutex = CreateMutex(NULL, false, name);
	}

	void Lock()
	{
		WaitForSingleObject(this->m_hMutex,INFINITE);
	}

	void Unlock()
	{
		ReleaseMutex(this->m_hMutex);
	}

private:
	HANDLE m_hMutex;
};

#endif // __MUTEX_H__