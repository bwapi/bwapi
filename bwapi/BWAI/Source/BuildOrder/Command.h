namespace BuildOrder
{
  /** Represens one task in the build order definition*/
  class Command
  {
    public :
      /** 
       * Tries to execut the current command 
       * @return @c true if the task was executed @false otherwise
       */
      virtual bool execute() = 0;
  };
}