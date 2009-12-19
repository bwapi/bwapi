#pragma once
/**
 *  Capsulation of the "packet id match" and
 *  "memory as packet type reinterpret" functionality
 *
 *  Usage:
 *  handlers (callbacks) are pushed
 *  one handler is called when processing a packet
 */

#include "MemoryFrame.h"
#include "Strings.h"
#include <map>

namespace Util
{
  template<typename RETTYPE>  // return type of handlers
  class TypedPacketSwitch
  {
  private:
    //--------------------------- ENTRIES ------------------------------------------
    struct HandlerEntry
    {
      RETTYPE (*reinterpreter)(void *, MemoryFrame&);
      void *functor;
    };
    typedef std::map<int, HandlerEntry> HandlerEntryMap;
    HandlerEntryMap handlers;
    //--------------------------- CASTER FUNCTION ----------------------------------
    template<typename TYPED_PACKET>
      static RETTYPE reinterpretMemoryAsPacket(void *functor, MemoryFrame& mem)
      {
        typedef RETTYPE (*HandlerFunc)(TYPED_PACKET&);
        HandlerFunc handler = *(HandlerFunc)functor;

        try
        {
          // call handler
          return (*handler)(mem.getAs<TYPED_PACKET>());
        }
        catch(GeneralException &exception)
        {
          exception.append(Strings::ssprintf("handling '%d' packet", TYPED_PACKET::_typeId));
          throw;
        }
      }
    //--------------------------- --------------------------------------------------
  public:
    //--------------------------- CONTRUCTION --------------------------------------
    TypedPacketSwitch()
    {
    }
    ~TypedPacketSwitch()
    {
    }
    //--------------------------- ADD HANDLER --------------------------------------
    template<typename TYPED_PACKET>
      void addHandler(RETTYPE (*handlerFunction)(TYPED_PACKET&))
      {
        HandlerEntry entry;
        entry.functor = (void*)handlerFunction;
        entry.reinterpreter = &reinterpretMemoryAsPacket<TYPED_PACKET>;
        handlers[TYPED_PACKET::_typeId] = entry;
        return;
      }
    //--------------------------- HANDLER COUNT ------------------------------------
    int getHandlerCount() const
    {
      return handlers.size();
    }
    //--------------------------- HANDLE PACKET ------------------------------------
    RETTYPE handlePacket(MemoryFrame& packet)
    {
      // read type
      int type = packet.getAs<int>();

      // find a handler
      HandlerEntryMap::iterator it = handlers.find(type);
      if(it == handlers.end())
        throw GeneralException(Strings::ssprintf("unhandled packet '%d'", type));

      // call custom callback
      HandlerEntry &entry = (*it).second;
      return (*entry.reinterpreter)(entry.functor, packet);
    }
    //--------------------------- --------------------------------------------------
  };
}
