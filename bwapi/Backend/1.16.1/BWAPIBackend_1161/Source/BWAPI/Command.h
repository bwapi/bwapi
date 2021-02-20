#pragma once
#include <BWAPI4/UnitCommand.h>

namespace BWAPI4
{
  // Forwards
  class UnitImpl;
  typedef UnitImpl* Unit;

  class PlayerImpl;

  class Command
  {
  public:
    Command(const UnitCommand& command) : command(command) { }

    void execute();
    void execute(bool isCurrentFrame);

    void insertIntoCommandBuffer(std::vector<std::vector<Command>>& buf)&&;

  private:
    enum struct EventType { Order, Resource, Finish };

    Command makeEvent(EventType type);
    static int getUnitID(Unit unit);

    UnitCommand command;
    EventType eventType = EventType::Resource;
    int savedExtra = -1;
    int savedExtra2 = -1;

    PlayerImpl* player = nullptr;
  };


};
