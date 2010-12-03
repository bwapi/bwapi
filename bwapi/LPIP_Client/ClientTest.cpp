// ClientTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"

#define BUFSIZE 512

int _tmain(int argc, _TCHAR* argv[])
{
	printf( "Starting Client...\n");
	Sleep(1000);

	std::wstring pipeName = L"\\\\.\\pipe\\sc_lpip_master";
	NamedPipes::Client *c = new NamedPipes::Client(pipeName,BUFSIZE);
	char* lpvMessage="Default message from client.\0";

	printf( "Sending %d byte message: \"%s\"\n", strlen(lpvMessage), lpvMessage);

	c->Send((BYTE*)lpvMessage,strlen(lpvMessage));

	printf("\nMessage sent to server, receiving reply as follows:\n");

	int received=0;
	BYTE* ret = c->Recieve(&received);

	printf("Message recieved:\n%s",ret);

	printf("\n<End of message, press ENTER to terminate connection and exit>");
	_getch();

	delete c;

	return 0;
}