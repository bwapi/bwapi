#ifndef __PIPE_H__
#define __PIPE_H__
#pragma once

namespace NamedPipes
{
	class Pipe
	{
	public:
		Pipe(HANDLE pipe)
		{
			this->m_hPipe = pipe;
		}

		~Pipe()
		{
			FlushFileBuffers(this->m_hPipe); 
			DisconnectNamedPipe(this->m_hPipe); 
			CloseHandle(this->m_hPipe);
			this->m_hPipe = INVALID_HANDLE_VALUE;
		}

		BOOL Peek(LPDWORD lpBytesAvail)
		{
			return PeekNamedPipe(this->m_hPipe,NULL,0,NULL,lpBytesAvail,NULL);
		}

		BOOL Read(BYTE* buffer, int size, LPDWORD bytesRead)
		{
			return ReadFile(this->m_hPipe,buffer,(DWORD)size,bytesRead,NULL);
		}

		DWORD Write(BYTE* buffer, int size, LPDWORD bytesWritten)
		{
			return WriteFile(this->m_hPipe,buffer,(DWORD)size,bytesWritten,NULL);
		}

		HANDLE GetHandle() { return this->m_hPipe; }

	private:
		HANDLE m_hPipe;
	};
}

#endif