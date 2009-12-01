
#define AGENT_INTERFACE virtual
#ifdef BWAGENT_EXPORTS
#define AGENT_API extern "C" __declspec(dllexport)
#else
#define AGENT_API extern "C" __declspec(dllimport)
#endif
