#include "Client.h"

NamedPipes::Client::Client(std::wstring pipeName, int szBufferSize)
{
	this->m_szBuffer = szBufferSize;
	BOOL   fSuccess = FALSE;
	DWORD  dwMode;

	// Try to open a named pipe; wait for it, if necessary.
	while (1)
	{
		this->m_hPipe = CreateFile(
			pipeName.c_str(),   // pipe name
			GENERIC_READ |  // read and write access
			GENERIC_WRITE,
			0,              // no sharing
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe
			0,              // default attributes
			NULL);          // no template file

		// Break if the pipe handle is valid.
		if (this->m_hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			printf( "Could not open pipe. GLE=%d\n", GetLastError() );
			return;
		}

		// All pipe instances are busy, so wait for 20 seconds.
		if ( !WaitNamedPipe(pipeName.c_str(), 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return;
		}
	}

	// The pipe connected; change to message-read mode.
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		this->m_hPipe,    // pipe handle
		&dwMode,  // new pipe mode
		NULL,     // don't set maximum bytes
		NULL);    // don't set maximum time

	if ( ! fSuccess)
	{
		printf( "SetNamedPipeHandleState failed. GLE=%d\n", GetLastError() );
		return;
	}
}

NamedPipes::Client::~Client()
{
	CloseHandle(this->m_hPipe);
}

void NamedPipes::Client::Send(BYTE *Data, int DataLen)
{
	int *written=new int;
	WriteFile( this->m_hPipe, Data, DataLen+1, (LPDWORD)&written, NULL);
}

BYTE* NamedPipes::Client::Recieve(int *bytesRecieved)
{
	BOOL fSuccess = FALSE;
	BYTE *buf = new BYTE[this->m_szBuffer];

	do 
	{
		
		// Read from the pipe.
		fSuccess = ReadFile(
			this->m_hPipe,    // pipe handle
			buf,    // buffer to receive reply
			this->m_szBuffer,  // size of buffer
			(LPDWORD)&bytesRecieved,  // number of bytes read
			NULL);    // not overlapped

		if (! fSuccess && GetLastError() != ERROR_MORE_DATA )
			break; 

		printf( "\"%s\"\nLength: %i\n", buf, bytesRecieved );
	} while ( !fSuccess);  // repeat loop if ERROR_MORE_DATA

	return buf;
}