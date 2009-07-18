#pragma once

namespace BuildOrder
{
  /** Type of behaviour of the build order command depending on it's condition. */
  namespace ConditionRunType
  {
    enum Enum
    {
      WaitToApply, /**< The build order is stopped and waits for the condition to be true */
      TryOnce,     /**< The build order tests the condition once, if it fails, it will skip the command. */
      KeepAndTry,  /**< The build order continues to execute other commands, but stores this one and tries to
                    *   execute it, once it's executed, is thrown away */
      TryForever,  /**< The build order continues to execute other commands, but stores this one and tries to
                    *   execute it, even if the condition pass and the command is executed it will be still
                    *   remain stored and can be executed again */
    };
  }
}
