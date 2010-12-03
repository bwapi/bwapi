// PipeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"

#define BUFSIZE 512

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Starting server...\n";
	NamedPipes::Server *s = new NamedPipes::Server(L"\\\\.\\pipe\\sc_lpip_master",BUFSIZE);
	DWORD lastAction=GetTickCount();
	int lastClients=0;

	while(true)
	{
		Sleep(1);
		int currClients = s->GetNumberOfClients();
		if( currClients != lastClients )
		{
			lastClients = currClients;
			if( currClients == 0 )
				lastAction=GetTickCount();
		}

		if( currClients > 0 )
		{
			NamedPipes::Packet *pkt=s->GetRequest();
			if( pkt == NULL )
				continue;

			lastAction=GetTickCount();

			char *str = (char*)pkt->Data;
			int id = pkt->ClientID;

			printf("Message received from client: \"%s\"\n",pkt->Data);

			std::string data = "Default message from server.";
			int len = data.length()+1;

			s->Send(id,(BYTE*)data.c_str(),len);
		}
		else
		{
			int diff = (int)(GetTickCount() - lastAction);
			if( diff > 30 * 1000 )
			{
				printf("Shutting down Server.");
				delete s;

				break;
			}
		}
	}

	return 0;
}