#ifndef __CLIENTINFO_H__
#define __CLIENTINFO_H__
#pragma once

#include "stdafx.h"
#include "Server.h"

namespace NamedPipes
{
	class Server;
	DWORD WINAPI ClientProcessThreadHelper(LPVOID p);
	DWORD WINAPI ClientReadThreadHelper(LPVOID p);

	class InfoPacket
	{
	public:
		int len;
		BYTE *data;
	};

	class ClientInfo
	{
	public:
		ClientInfo(Server *s, HANDLE pipe, int id);
		~ClientInfo();

		void EnqueueResponse(BYTE *data, int datalen);
		InfoPacket* DequeueRequest();

		void StartClientThreads();

		int getID() { return this->m_ID; }

	private:
		ClientInfo(const ClientInfo&) {}		// Private copy constructor

		void ProcessThread();
		void ReadThread();

		int Send(InfoPacket *pkt);

		Server *m_pServer;
		Pipe *m_hPipe;
		int m_ID;

		Collections::Queue<InfoPacket*> *m_lResponses;
		Collections::Queue<InfoPacket*> *m_lRequests;

		Mutex *m_mReqs, *m_mResps, *m_mPipe;

		HANDLE m_hReadThread,m_hProcessThread;

		friend DWORD WINAPI ClientProcessThreadHelper(LPVOID ci);
		friend DWORD WINAPI ClientReadThreadHelper(LPVOID ci);
	};
}

#endif // __CLIENTINFO_H__