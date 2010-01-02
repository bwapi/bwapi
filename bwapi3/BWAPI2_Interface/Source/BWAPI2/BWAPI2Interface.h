// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BWAPI2_INTERFACE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BWAPI2_EXPORT functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef BWAPI2_INTERFACE_EXPORTS
#define BWAPI2_EXPORT __declspec(dllexport)
#else
#define BWAPI2_EXPORT __declspec(dllimport)
#endif

// tell interface packer to strip the file of private declarations,
// so they don't clutter the headers
#define IP_STRIP

// use at any line to tell IP to ignore all successing lines
// untill AGENT_STRIP or AGENT_TRANSIT
#define IP_IGNORE

// resets IP to default "all copying" state after this line
#define IP_TRANSIT
