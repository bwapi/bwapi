
#define AGENT_INTERFACE virtual
#ifdef BWAGENT_EXPORTS
#define AGENT_API __declspec(dllexport)
#else
#define AGENT_API __declspec(dllimport)
#endif

// use somewhere at the beginning of file to tell
// interface packer to strip the file of not-exported
// functions, and convert virtuals to pure
#define AGENT_STRIP
