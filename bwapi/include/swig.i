/*
 swig -java -v -package bwapi -outdir java -Wall -c++ -debug-tmsearch swig.i > output.txt

*/

%include <std_common.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>
%include <std_pair.i>
%include <std_list.i>
%include <std_deque.i>
%include <std_function.i>

%include <common.i>
%include <Position.i>
%include <Type.i>

%module BWAPICore
%{
#include "BWAPI.h"
%}

// Causes issues, so remove it
#define static_assert(...)

// Define BWAPI Positions
position_template(Position)
position_template(TilePosition)
position_template(WalkPosition)

type_template(BulletType)
type_template(DamageType)
type_template(Error)
type_template(ExplosionType)
type_template(GameType)
type_template(Order)
type_template(PlayerType)
type_template(Race)
type_template(TechType)
type_template(UnitCommandType)
type_template(UnitSizeType)
type_template(UnitType)
type_template(UpgradeType)
type_template(WeaponType)

// --------------------------------------------------------------------

%define INTERFACE_RENAME(t)
  %std_function(t ## ActionCB, void, BWAPI::t*);
  %std_function(t ## ConditionCB, bool, BWAPI::t*);
%enddef

%define INTERFACE_TEMPLATE(t)
  %template(t ## Base) BWAPI::Interface<BWAPI::t>;
  //%template(t ## ActionCB), std::function<void(BWAPI::t)
%enddef

// ----------------------------------------------------------------------------

// Global operator remapping
%ignore operator =;
%rename(equals) operator ==;
%ignore operator !=;
%ignore operator &=;
%ignore operator &;
%ignore operator ^=;
%ignore operator ^;
%ignore operator /;
%ignore operator /=;
%ignore operator %;
%ignore operator %=;
%ignore operator <<;
%ignore operator >>;
%ignore operator bool;
%ignore operator int;
%ignore operator +;
%ignore operator +=;
%ignore operator -;
%ignore operator -=;
%ignore operator *;
%ignore operator *=;
%ignore operator |;
%ignore operator |=;
%ignore operator &;
%ignore operator &=;
%ignore operator <;
%ignore operator >;
%ignore operator <=;
%ignore operator >=;


// ------------------------------------------------------------------------------

%rename(Text) BWAPI::Text::Enum;
%rename(TextSize) BWAPI::Text::Size::Enum;
%rename(CoordinateType) BWAPI::CoordinateType::Enum;
%rename(EventType) BWAPI::EventType::Enum;
%rename(Flag) BWAPI::Flag::Enum;
%rename(MouseButton) BWAPI::MouseButton::Enum;
%rename(Key) BWAPI::Key::Enum;
%rename(Latency) BWAPI::Latency::Enum;
%rename(TournamentAction) BWAPI::Tournament::ActionID;

INTERFACE_RENAME(BulletInterface)
INTERFACE_RENAME(ForceInterface)
INTERFACE_RENAME(Game)
INTERFACE_RENAME(PlayerInterface)
INTERFACE_RENAME(RegionInterface)
INTERFACE_RENAME(UnitInterface)

%include "BWAPI/InterfaceEvent.h"
%include "BWAPI/Interface.h"
%include "BWAPI/SetContainer.h"
%include "BWAPI/Type.h"
%include "BWAPI/Position.h"
%include "BWAPI/EventType.h"
%include "BWAPI/Event.h"
%include "BWAPI/GameType.h"
%include "BWAPI/Race.h"
%include "BWAPI/TechType.h"
%include "BWAPI/UpgradeType.h"
%include "BWAPI/AIModule.h"
%include "BWAPI/Color.h"
%include "BWAPI/Constants.h"
%include "BWAPI/CoordinateType.h"
%include "BWAPI/DamageType.h"
%include "BWAPI/Error.h"
%include "BWAPI/ExplosionType.h"
%include "BWAPI/Flag.h"
%include "BWAPI/Force.h"
%include "BWAPI/Forceset.h"
%include "BWAPI/Input.h"
%include "BWAPI/Latency.h"
%include "BWAPI/Order.h"
%include "BWAPI/PlayerType.h"
%include "BWAPI/Player.h"
%include "BWAPI/Playerset.h"
%include "BWAPI/BulletType.h"
%include "BWAPI/Bullet.h"
%include "BWAPI/Bulletset.h"
%include "BWAPI/UnitCommandType.h"
%include "BWAPI/UnitCommand.h"
%include "BWAPI/Game.h"
%include "BWAPI/Unit.h"
%include "BWAPI/Unitset.h"
%include "BWAPI/UnitSizeType.h"
%include "BWAPI/UnitType.h"
%include "BWAPI/WeaponType.h"
%include "BWAPI/Region.h"
%include "BWAPI/Regionset.h"
%include "BWAPI.h"


INTERFACE_TEMPLATE(BulletInterface)
INTERFACE_TEMPLATE(ForceInterface)
INTERFACE_TEMPLATE(Game)
INTERFACE_TEMPLATE(PlayerInterface)
INTERFACE_TEMPLATE(RegionInterface)
INTERFACE_TEMPLATE(UnitInterface)

%template(ColorBase) BWAPI::Type<BWAPI::Color,255>;

%template(EventList) std::list<BWAPI::Event>;
%template(UnitTypeIntPair) std::pair<BWAPI::UnitType, int>;
%template(UnitTypeIntMap) std::map<BWAPI::UnitType, int>;

