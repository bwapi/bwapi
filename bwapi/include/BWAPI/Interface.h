#pragma once
#include <map>
#include <list>

#include <BWAPI/InterfaceEvent.h>

namespace BWAPI
{
  /// This generalized class allows the application of features that are common to all interface
  /// classes.
  template < typename T >
  class Interface
  {
  protected:
    // default ctor
    Interface() {};
    // dtor
    virtual ~Interface()
    {};

    std::map<int,void*> clientInfo;
    std::list< InterfaceEvent<T> > interfaceEvents;

    friend class GameImpl;

    // Function manages events and updates it for the given frame
    void updateEvents()
    {
      auto e = interfaceEvents.begin();
      while ( e != interfaceEvents.end() )
      {
        if ( e->isFinished() )
        {
          e = interfaceEvents.erase(e);
        }
        else
        {
          e->execute(static_cast<T*>(this));
          ++e;
        }
      }
    };
  public:
    /// Retrieves a pointer or value at an index that was stored for this interface using
    /// setClientInfo.
    ///
    /// @param key
    ///   The key containing the value to retrieve. Default is 0.
    /// 
    /// @retval nullptr if index is out of bounds.
    /// @returns The client info at the given index.
    /// @see setClientInfo
    void *getClientInfo(int key = 0) const
    {
      // Retrieve iterator to element at index
      auto result = this->clientInfo.find(key);

      // Return a default value if not found
      if ( result == this->clientInfo.end() )
        return nullptr;

      // return the desired value
      return result->second;
    };

    template <typename CT>
    CT getClientInfo(int key = 0) const
    {
      return (CT)(int)this->getClientInfo(key);
    };

    /// Associates one or more pointers or values with any BWAPI interface.
    ///
    /// This client information is managed entirely by the AI module. It is not modified by BWAPI.
    /// @warning If a pointer to allocated memory is used, then the AI module is responsible for
    /// deallocating the memory when the game ends.
    ///
    /// If client info at the given index has already been set, then it will be overwritten.
    ///
    /// @param clientInfo
    ///   The data to associate with this interface.
    /// @param key
    ///   The key to use for this data. Default is 0.
    ///
    /// @see getClientInfo
    template < typename V >
    void setClientInfo(const V &clientInfo, int key = 0)
    {
      this->clientInfo[key] = (void*)clientInfo;
    };

    /// Registers an event and associates it with the current Interface object. Events can be used
    /// to automate tasks (like train X @Marines until Y of them have been created by the given
    /// @Barracks) or to create user-defined callbacks.
    ///
    /// @param action
    ///   The action callback to be executed when the event conditions are true. It is of type
    ///   void fn(T *inst) where fn is the function name and inst is a pointer to the instance of
    ///   the object performing the action.
    /// @param condition (optional)
    ///   The condition callback which will return true if the action is intended to be executed.
    ///   It is of type bool fn(T *inst) where fn is the function name and inst is a pointer to the
    ///   instance of the object performing the check. The condition will always be true if omitted.
    /// @param timesToRun (optional)
    ///   The number of times to execute the action before the event is removed. If the value is
    ///   negative, then the event will never be removed. The value will be -1 if omitted, causing
    ///   the event to execute regularly.
    /// @param framesToCheck (optional)
    ///   The number of frames to skip between checks. If this value is 0, then a condition check is
    ///   made once per frame. If this value is 1, then the condition for this event is only checked
    ///   every other frame. This value is 0 by default, meaning the event's condition is checked
    ///   every frame.
    void registerEvent(const std::function<void(T*)> &action, const std::function<bool(T*)> &condition = nullptr, int timesToRun = -1, int framesToCheck = 0)
    {
      interfaceEvents.push_back( InterfaceEvent<T>(action,condition,timesToRun,framesToCheck) );
    };
    /// @overload
    void registerEvent(const InterfaceEvent<T> &evt)
    {
      interfaceEvents.push_back( evt );
    };
    /// @overload
    void registerEvent(InterfaceEvent<T> &&evt)
    {
      interfaceEvents.push_back( std::move(evt) );
    };

  };


}
