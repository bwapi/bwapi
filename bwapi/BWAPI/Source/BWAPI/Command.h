#pragma once
#include "CommandTypes.h"
#include <vector>
namespace BWAPI
{
  class UnitImpl;
  /**
   * BWAPI internal representation of command. Every command ordered is
   * represented by descendant of this class and stored in the
   * Game#commandBuffer so we can make Local version of data.
   */
  class Command
  {
    public :
      /**
       * Makes it's own copy of executor list.
       * @param executors Executors of the command (Every command has some
       *        executor).
       */
      Command(const std::vector<UnitImpl*>& executors);
      /**
       * Special case with one executor only, makes list containing the
       * specified value.
       * @param executor Executor of the command.
       */
      Command(UnitImpl* executor);
      virtual ~Command();
      /** Gets the type of the command  */
      virtual BWAPI::CommandTypes::Enum getType() = 0;
      /**
       * Executes the current command. Note that this command will only affect
       * data with Local postfix.
       */
      virtual void execute() = 0;
      /** This value specifies if the last execution of the command failed */
      bool failed;
      virtual std::string describe() = 0;
    protected :
      /** List of executors of the command. */
      std::vector<UnitImpl*> executors;
  };
}