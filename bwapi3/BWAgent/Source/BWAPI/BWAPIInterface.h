
#ifdef BWAGENT_EXPORTS
#define BWAPI_FUNCTION __declspec(dllexport)
#define BWAPI_METHOD __declspec(dllexport)
#define BWAPI_CLASS __declspec(dllexport)
#else
#define BWAPI_FUNCTION __declspec(dllimport)
#define BWAPI_METHOD __declspec(dllimport)
#define BWAPI_CLASS __declspec(dllimport)
#endif

