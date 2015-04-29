#pragma once
#include <functional>

namespace BWAPI
{
  /// <summary>The interface event is an attachment to BWAPI interfaces which allows the user to
  /// register powerful conditional callbacks that can be executed a user-specified number of
  /// times.</summary>
  ///
  /// @tparam T
  ///     The interface that this class is being used for.
  ///
  /// @see Interface::registerEvent
  template < typename T >
  class InterfaceEvent
  {
  public:
    InterfaceEvent() = default;

    /// <summary>This is the expected constructor for an interface event.</summary>
    ///
    /// <param name="action">
    ///     An function callback that is run when all other conditions for this event are met.
    /// </param>
    /// <param name="condition"> (optioinal)
    ///     A function predicate that returns true if this event should be triggered.
    /// </param>
    /// <param name="timesToRun"> (optional)
    ///     The number of times that the action is called before this event is discarded. A value
    ///     of -1 means the event will never be discarded until the game has ended.
    /// </param>
    /// <param name="framesToCheck"> (optional)
    ///     The frequency that the event will check its condition at. A lower value indicates a
    ///     higher frequency. A value of 0 means that the condition is checked every frame.
    ///     A value of 10 means that the condition is checked once every 10 frames.
    /// </param>
    InterfaceEvent(const std::function<void(T*)> &action, const std::function<bool(T*)> &condition = nullptr, int timesToRun = -1, int framesToCheck = 0)
      : condProc( condition )
      , execProc( action )
      , runFreq( framesToCheck )
      , runCount( timesToRun )
      , step( framesToCheck )
    {};

    /// <summary>Checks if the event has finished its execution and is marked for removal.</summary>
    ///
    /// @returns true if the event has completed all runs and/or is marked for removal, otherwise
    /// it returns false if the event should continue execution.
    bool isFinished() const
    {
      return this->runCount == 0;
    };

    /// <summary>Marks the event for removal.</summary>
    void removeEvent()
    {
      this->runCount = 0;
    };

  protected:
    template < typename U >
    friend class Interface;

    // Function that runs the event, checkings its conditions and running its action, then
    // decrementing the run count.
    bool execute(T *instance)
    {
      // condition check
      if ( !this->isConditionMet(instance) )
        return false;

      // There must be an exec proc!
      if ( !this->execProc )
        return false;

      // execute
      this->execProc(instance);

      // Decrement run count (-1 being infinite)
      if ( this->runCount > 0 )
        --this->runCount;
      return true;
    };

    // Function to check if the condition associated with the event is true. Includes frame and
    // run count checking.
    bool isConditionMet(T *instance)
    {
      // Validity check
      if ( this->isFinished() )
        return false;

      // Frame check
      --step;
      if ( step > 0 )
        return false;
      this->step = this->runFreq;

      // Conditional check
      if ( this->condProc )
        return this->condProc(instance);
      return true; // always run if there is no conditional function
    };
  private:
    // Data members
    std::function<bool(T*)> condProc = nullptr;
    std::function<void(T*)> execProc = nullptr;
    int runFreq = 0;  // Frequency of runs, in frames (0 means every frame, 1 means every other frame)
    int runCount = 0; // Number of times that the action can occur (-1 being infinite)
    int step = 0;     // Current step. Executes when reaches 0, then reset to runFreq.
  };
}
