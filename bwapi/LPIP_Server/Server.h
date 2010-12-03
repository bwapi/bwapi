#ifndef __SERVER_H__
#define __SERVER_H__
#pragma once

#include "stdafx.h"
#include "ClientInfo.h"

namespace NamedPipes
{
	struct Packet
	{
		int ClientID;
		int DataLength;
		BYTE *Data;
	};
	class ClientInfo;

	DWORD WINAPI ListenThreadHelper(LPVOID p);

	class Server
	{
	public:
		Server(std::wstring pipeName, int szBufferSize);
		~Server();

		int BufferSize() { return this->m_szBuffers; }

		Packet* GetRequest();
		void Send(Packet *pkt);
		void Send(int ClientID, BYTE *Data, int DataLength)
		{
			Packet pkt;
			pkt.ClientID = ClientID;
			pkt.DataLength = DataLength;
			pkt.Data = Data;
			Send(&pkt);
		}

		void RemoveClient(ClientInfo *ci);

		int GetNumberOfClients(){ return this->m_lClients->GetCount(); }

	private:
		Server(const Server&) {}		// Private copy constructor

		void StartListenThread();
		void ListenThread();

		HANDLE m_hPipe;
		std::wstring m_strPipe;
		int m_szBuffers;
		int m_nextID;

		Collections::LinkedList<ClientInfo*> *m_lClients;

		HANDLE m_hListenThread;

		friend DWORD WINAPI ListenThreadHelper(LPVOID func);
		friend DWORD WINAPI ClientThreadHelper(LPVOID p);
	};
}

#endif // __SERVER_H__