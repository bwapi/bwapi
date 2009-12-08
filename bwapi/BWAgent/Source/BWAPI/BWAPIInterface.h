
#define BWAPI_METHOD virtual
#ifdef BWAGENT_EXPORTS
#define BWAPI_FUNCTION extern "C" __declspec(dllexport)
#else
#define BWAPI_FUNCTION extern "C" __declspec(dllimport)
#endif

#define BWAPI_CALL __stdcall
