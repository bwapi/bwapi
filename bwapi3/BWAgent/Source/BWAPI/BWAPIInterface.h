
#define BWAPI_METHOD virtual
#ifdef BWAGENT_EXPORTS
#define BWAPI_FUNCTION __declspec(dllexport)
#else
#define BWAPI_FUNCTION __declspec(dllimport)
#endif

