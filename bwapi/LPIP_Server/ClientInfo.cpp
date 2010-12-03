#include "ClientInfo.h"

NamedPipes::ClientInfo::ClientInfo(Server *s, HANDLE pipe, int id)
{
	this->m_pServer = s;
	this->m_hPipe = new Pipe(pipe);
	this->m_ID = id;
	
	this->m_lResponses = new Collections::Queue<InfoPacket*>();
	this->m_lRequests = new Collections::Queue<InfoPacket*>();

	this->m_mReqs = new Mutex(NULL);
	this->m_mResps = new Mutex(NULL);
	this->m_mPipe = new Mutex(NULL);
}

NamedPipes::ClientInfo::~ClientInfo()
{
	TerminateThread(this->m_hProcessThread,1);
	CloseHandle(this->m_hProcessThread);
	TerminateThread(this->m_hReadThread,1);
	CloseHandle(this->m_hReadThread);

	this->m_mPipe->Lock();
	delete this->m_hPipe;
	this->m_mPipe->Unlock();
}

void NamedPipes::ClientInfo::EnqueueResponse(BYTE *data, int datalen)
{
	InfoPacket *pkt = new InfoPacket();
	pkt->len=datalen;
	pkt->data=(BYTE*)(new char[datalen]);
	memcpy(pkt->data,data,datalen);

	this->m_mResps->Lock();
	this->m_lResponses->Enqueue(pkt);
	this->m_mResps->Unlock();
}

NamedPipes::InfoPacket* NamedPipes::ClientInfo::DequeueRequest()
{
	if( this->m_lRequests->GetCount() <= 0 )
		return NULL;

	this->m_mReqs->Lock();
	InfoPacket *ret = this->m_lRequests->Dequeue();
	this->m_mReqs->Unlock();

	return ret;
}

int NamedPipes::ClientInfo::Send(InfoPacket *pkt)
{
	DWORD bytesWritten=0;

	this->m_mPipe->Lock();
	DWORD fSuccess = this->m_hPipe->Write(pkt->data,pkt->len,&bytesWritten); 
	this->m_mPipe->Unlock();

	if (!fSuccess || bytesWritten != pkt->len)
		Utils::Logger::LogError("ClientProcessThread WriteFile failed",this->m_ID); 

	return (int)bytesWritten;
}

DWORD WINAPI NamedPipes::ClientProcessThreadHelper(LPVOID ci)
{
	ClientInfo *info = (ClientInfo*)ci;
	info->ProcessThread();

	return 1;
}

DWORD WINAPI NamedPipes::ClientReadThreadHelper(LPVOID ci)
{
	ClientInfo *info = (ClientInfo*)ci;
	info->ReadThread();

	return 1;
}

void NamedPipes::ClientInfo::StartClientThreads()
{
	this->m_hReadThread=CreateThread(NULL, 0, NamedPipes::ClientReadThreadHelper, this, 0, NULL);
	this->m_hProcessThread=CreateThread(NULL, 0, NamedPipes::ClientProcessThreadHelper, this, 0, NULL);
}

void NamedPipes::ClientInfo::ReadThread()
{
	BYTE* pchRequest = new BYTE[this->m_pServer->BufferSize()];

	DWORD cbBytesRead = 0, cbWritten = 0; 
	BOOL fSuccess = FALSE;

	// Print verbose messages. In production code, this should be for debugging only.
	Utils::Logger::Log("ClientListenThread created, receiving and processing messages.",this->m_ID);

	// Loop until done reading
	while (true) 
	{ 
		DWORD bytesAvail;
		this->m_mPipe->Lock();
		fSuccess = this->m_hPipe->Peek(&bytesAvail);
		this->m_mPipe->Unlock();

		if (!fSuccess)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				Utils::Logger::Log("ClientListenThread: client disconnected.",this->m_ID);
			}
			else
			{
				Utils::Logger::LogError("ClientListenThread PeekNamedPipe failed.",this->m_ID);
			}
			delete pchRequest;
			this->m_pServer->RemoveClient(this);
			break;
		}

		if( bytesAvail > 0 )
		{
			cbBytesRead = 0;
			// Read client requests from the pipe. This simplistic code only allows messages
			// up to BUFSIZE characters in length.
			this->m_mPipe->Lock();
			fSuccess = this->m_hPipe->Read(pchRequest,this->m_pServer->BufferSize(),&cbBytesRead); 
			this->m_mPipe->Unlock();

			if (!fSuccess || cbBytesRead == 0)
			{
				if (GetLastError() == ERROR_BROKEN_PIPE)
				{
					Utils::Logger::Log("ClientListenThread: client disconnected.",this->m_ID);
				}
				else
				{
					Utils::Logger::LogError("ClientListenThread ReadFile failed.",this->m_ID);
				}
				delete pchRequest;
				this->m_pServer->RemoveClient(this);
				break;
			}

			// save pchRequest to an InfoPacket and store in m_lRequests
			InfoPacket *pkt = new InfoPacket();
			pkt->len=cbBytesRead;
			pkt->data=pchRequest;
			this->m_mReqs->Lock();
			this->m_lRequests->Enqueue(pkt);
			this->m_mReqs->Unlock();
		}

		Sleep(1);
	}
}

void NamedPipes::ClientInfo::ProcessThread()
{
	while(true)
	{
		if( this->m_lResponses->GetCount() > 0 )
		{
			this->m_mResps->Lock();
			InfoPacket *pkt = this->m_lResponses->Dequeue();
			this->m_mResps->Unlock();

			this->Send(pkt);
		}

		Sleep(1);
	}
}