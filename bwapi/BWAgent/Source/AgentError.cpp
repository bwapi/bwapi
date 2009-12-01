#include "AgentError.h"

namespace BWAgent
{
  std::string lastError = "No Error";

  void resetError()
  {
    lastError = "No Error";
  }
}
