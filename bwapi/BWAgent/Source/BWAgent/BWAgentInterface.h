
#define AGENT_INTERFACE virtual
#ifdef BWAGENT_EXPORTS
#define AGENT_API __declspec(dllexport)
#else
#define AGENT_API __declspec(dllimport)
#endif

// use somewhere at the beginning of file to tell
// interface packer to strip the file of not-exported
// functions, and convert virtuals to pure, after this
// line
#define AGENT_STRIP

// use at any line to tell IP to ignore all successing lines
// untill AGENT_STRIP or AGENT_TRANSIT
#define AGENT_IGNORE

// resets IP to default "all copying" state after this line
#define AGENT_TRANSIT
