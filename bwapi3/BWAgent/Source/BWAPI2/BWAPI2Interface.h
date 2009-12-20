
#define BWAPI2_METHOD virtual
#ifdef BWAGENT_EXPORTS
#define BWAPI2_FUNCTION extern "C" __declspec(dllexport)
#else
#define BWAPI2_FUNCTION extern "C" __declspec(dllimport)
#endif

#define BWAPI2_CALL __stdcall

// use somewhere at the beginning of file to tell
// interface packer to strip the file of not-exported
// functions, and convert virtuals to pure, after this
// line
#define IP_STRIP

// use at any line to tell IP to ignore all successing lines
// untill AGENT_STRIP or AGENT_TRANSIT
#define IP_IGNORE

// resets IP to default "all copying" state after this line
#define IP_TRANSIT
