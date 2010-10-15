
namespace Util
{
  /** The level of detail of the log. */
  namespace LogLevel
  {
    enum Enum
    {
      DontLog       = 0, /**< No logs will be printed by logger with this logLevel specification. Shouldn't be used by the Logger#log for obvious reasons. */
      Critical      = 1, /**< Mostly errors, very important. */
      Important     = 2, /**< Bigger events. */
      Normal        = 2, /**< Normal events, like commands ordered etc. */
      Commmon       = 3, /**< Common things. */
      Detailed      = 4, /**< Detailed events mainly for investigation of problems. */
      MicroDetailed = 5  /**< Super often occuring event's like calling common functions etc, used mainly for searching bugs. */
    };
  }
}
