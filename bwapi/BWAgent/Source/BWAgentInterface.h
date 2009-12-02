
#define AGENT_INTERFACE virtual
#ifdef BWAGENT_EXPORTS
#define AGENT_API __declspec(dllexport)
#else
#define AGENT_API __declspec(dllimport)
#endif
