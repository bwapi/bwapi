#include "Server.h"

NamedPipes::Server::Server(std::wstring pipeName, int szBufferSize)
{
	this->m_hPipe = INVALID_HANDLE_VALUE;
	this->m_strPipe = pipeName;
	this->m_szBuffers = szBufferSize;
	this->m_nextID = 1;

	this->m_lClients = new Collections::LinkedList<ClientInfo*>();

	this->StartListenThread();
}

NamedPipes::Server::~Server()
{
	TerminateThread(this->m_hListenThread,1);
	CloseHandle(this->m_hListenThread);

	for( int i=this->m_lClients->GetCount()-1; i>=0; i-- )
	{
		ClientInfo *ci = this->m_lClients->Get(i);
		Utils::Logger::Log("Disconnecting Client.",ci->getID());
		this->m_lClients->Remove(ci);
		delete ci;
	}

	this->m_hPipe = INVALID_HANDLE_VALUE;
	this->m_strPipe = L"";
}

void NamedPipes::Server::RemoveClient(ClientInfo *ci)
{
	Utils::Logger::Log("Client Disconnected.",ci->getID());
	this->m_lClients->Remove(ci);
	delete ci;
}

NamedPipes::Packet* NamedPipes::Server::GetRequest()
{
	Collections::IEnumerator<ClientInfo*> *i = this->m_lClients->GetEnumerator();

	while(i->MoveNext())
	{
		InfoPacket *ret = i->GetCurrent()->DequeueRequest();
		if( ret == NULL )
			continue;

		Packet pkt;
		pkt.ClientID = i->GetCurrent()->getID();
		pkt.DataLength = ret->len;
		pkt.Data = ret->data;

		return &pkt;
	}

	return NULL;
}

void NamedPipes::Server::Send(Packet *pkt)
{
	Collections::IEnumerator<ClientInfo*> *i = this->m_lClients->GetEnumerator();

	while(i->MoveNext())
	{
		ClientInfo *ci = i->GetCurrent();
		if( ci->getID() != pkt->ClientID )
			continue;

		ci->EnqueueResponse(pkt->Data,pkt->DataLength);
		return;
	}
}

DWORD WINAPI NamedPipes::ListenThreadHelper(LPVOID p)
{
	((Server*)p)->ListenThread();

	return 1;
}

void NamedPipes::Server::StartListenThread()
{
	this->m_hListenThread = CreateThread(NULL, 0, NamedPipes::ListenThreadHelper, this, 0, NULL);
}

void NamedPipes::Server::ListenThread()
{
	while(true)
	{
		HANDLE hPipe;

		hPipe = CreateNamedPipe( 
			this->m_strPipe.c_str(),	// pipe name 
			PIPE_ACCESS_DUPLEX,			// read/write access 
			PIPE_TYPE_MESSAGE |			// message type pipe 
			PIPE_READMODE_MESSAGE |		// message-read mode 
			PIPE_WAIT,					// blocking mode 
			PIPE_UNLIMITED_INSTANCES,	// max. instances  
			this->m_szBuffers,			// output buffer size 
			this->m_szBuffers,			// input buffer size 
			0,							// client time-out 
			NULL);						// default security attribute 

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			Utils::Logger::LogError("CreateNamedPipe failed.",0);
			return;
		}

		bool flConnected = ( (ConnectNamedPipe(hPipe, NULL)) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED));
		
		if( flConnected )
		{
			ClientInfo *ci = new ClientInfo(this,hPipe,this->m_nextID++);
			ci->StartClientThreads();
			this->m_lClients->Add(ci);
		}
	}
}