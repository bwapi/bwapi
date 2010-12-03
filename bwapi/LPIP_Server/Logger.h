#ifndef __LOGGER_H__
#define __LOGGER_H__
#pragma once

namespace Utils
{
	namespace Logger
	{
		static void LogError(std::string err, int id)
		{
			int gle = GetLastError();
			printf(err.insert(0,"ERROR: ").insert(0,"[%i] ").append(" [0x%08X][%i]\n").c_str(),id,gle,gle);
		}

		static void Log(std::string msg, int id)
		{
			printf(msg.insert(0,"MESSAGE: ").insert(0,"[%i] ").append("\n").c_str(),id);
		}
	}
}

#endif // __LOGGER_H__