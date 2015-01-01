#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <BWAPI.h>
#include "Replay.h"
#include "ReplayTool.h"
#include "ParseReplayParams.h"

ReplayTool::ParseReplayParams g_options;

enum ToolArgs
{
  ARG_EXE = 0,
  ARG_Option,
  ARG_File,
  ARG_REPO,
  ARG_COUNT
};

void Message(LPCSTR pszText, LPCSTR pszCaption)
{
  std::cout << pszCaption << ": " << pszText << std::endl;
}

int Usage()
{
  MessageBoxA(NULL,  "Usage: Replay_Tool -[option] [replay-path] [output-dir-path]\n"
    "[option]"
    " -u Unpack replay.\n"
    " -p Pack replay.\n"
    " -r Auto-Repair replay.\n"
    "\nAliases:\n"
    " -e Extract. Alias for unpack.\n"
    " -d Decompress. Alias for unpack.\n"
    " -c Compress. Alias for pack.\n"
    " -f Fix. Alias for repair.\n",
    "Usage", MB_OK | MB_ICONINFORMATION);

  return EXIT_SUCCESS;
}

int main(int argc, LPTSTR argv[])
{
  ReplayTool::init();

  // Invalid number of arguments
  if (argc < ARG_COUNT)
    return Usage();

  size_t optionLen = strlen(argv[ARG_Option]);

  if (optionLen != 2)
  {
    Message("Invalid option", "Fail");
    return 0;
  }
  else
    g_options.setOption(argv[ARG_Option][1]);

  if (argv[ARG_File] == NULL || argv[ARG_File][0] == '\0')
  {
    Message("Invalid filename", "Fail");
    return 0;
  }
  else
  {
    g_options.setReplayPath(argv[ARG_File]);
  }

  if (argv[ARG_REPO] == NULL || argv[ARG_REPO][0] == '\0')
  {
    Message("Invalid repository", "Fail");
    return 0;
  }
  else
  {
    g_options.setOutRepoPath(argv[ARG_REPO]);
  }

  switch (g_options.getOption())
  {
  case 'u':
  case 'U':
  case 'e':
  case 'E':
  case 'd':
  case 'D':
    if (parseReplay(g_options, RFLAG_EXTRACT))
      Message("Extracted successfully.", "Success");
    else
      Message("Failed reading process somewhere.", "Failure");
    break;
  case 'p':
  case 'P':
  case 'c':
  case 'C':
    break;
  case 'r':
  case 'R':
  case 'f':
  case 'F':
    if (parseReplay(g_options, RFLAG_REPAIR))
      Message("Repaired successfully.", "Success");
    else
      Message("Failed reading process somewhere.", "Failure");
    break;
  default:
    return Usage();
  }

  return EXIT_SUCCESS;
}