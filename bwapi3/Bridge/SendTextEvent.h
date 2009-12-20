#pragma once

#include <Util\Hybrid.h>

namespace Bridge
{
  struct SendTextEventHead
  {
    // data to be sent along
  };
  typedef Util::Hybrid<SendTextEventHead,char> SendTextEvent;
}
