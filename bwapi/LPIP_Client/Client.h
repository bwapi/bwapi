#ifndef __CLIENT_H__
#define __CLIENT_H__
#pragma once

#include "stdafx.h"

namespace NamedPipes
{
	class Client
	{
	public:
		Client(std::wstring pipeName, int szBufferSize);
		~Client();

		void Send(BYTE *Data, int DataLen);
		BYTE* Recieve(int* bytesRead);

	private:
		Client(const Client&) {}			// Private copy constructor

		HANDLE m_hPipe;
		int m_szBuffer;
	};
}

#endif