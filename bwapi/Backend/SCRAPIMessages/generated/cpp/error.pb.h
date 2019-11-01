// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: error.proto

#ifndef PROTOBUF_INCLUDED_error_2eproto
#define PROTOBUF_INCLUDED_error_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_error_2eproto 

namespace protobuf_error_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_error_2eproto
namespace SCRAPIProtocol {
}  // namespace SCRAPIProtocol
namespace SCRAPIProtocol {

enum ActionResult {
  Success = 1,
  NotSupported = 2,
  Error = 3,
  CantQueueThatOrder = 4,
  Retry = 5,
  Cooldown = 6,
  QueueIsFull = 7,
  RallyQueueIsFull = 8,
  NotEnoughMinerals = 9,
  NotEnoughVespene = 10,
  NotEnoughTerrazine = 11,
  NotEnoughCustom = 12,
  NotEnoughFood = 13,
  FoodUsageImpossible = 14,
  NotEnoughLife = 15,
  NotEnoughShields = 16,
  NotEnoughEnergy = 17,
  LifeSuppressed = 18,
  ShieldsSuppressed = 19,
  EnergySuppressed = 20,
  NotEnoughCharges = 21,
  CantAddMoreCharges = 22,
  TooMuchMinerals = 23,
  TooMuchVespene = 24,
  TooMuchTerrazine = 25,
  TooMuchCustom = 26,
  TooMuchFood = 27,
  TooMuchLife = 28,
  TooMuchShields = 29,
  TooMuchEnergy = 30,
  MustTargetUnitWithLife = 31,
  MustTargetUnitWithShields = 32,
  MustTargetUnitWithEnergy = 33,
  CantTrade = 34,
  CantSpend = 35,
  CantTargetThatUnit = 36,
  CouldntAllocateUnit = 37,
  UnitCantMove = 38,
  TransportIsHoldingPosition = 39,
  BuildTechRequirementsNotMet = 40,
  CantFindPlacementLocation = 41,
  CantBuildOnThat = 42,
  CantBuildTooCloseToDropOff = 43,
  CantBuildLocationInvalid = 44,
  CantSeeBuildLocation = 45,
  CantBuildTooCloseToCreepSource = 46,
  CantBuildTooCloseToResources = 47,
  CantBuildTooFarFromWater = 48,
  CantBuildTooFarFromCreepSource = 49,
  CantBuildTooFarFromBuildPowerSource = 50,
  CantBuildOnDenseTerrain = 51,
  CantTrainTooFarFromTrainPowerSource = 52,
  CantLandLocationInvalid = 53,
  CantSeeLandLocation = 54,
  CantLandTooCloseToCreepSource = 55,
  CantLandTooCloseToResources = 56,
  CantLandTooFarFromWater = 57,
  CantLandTooFarFromCreepSource = 58,
  CantLandTooFarFromBuildPowerSource = 59,
  CantLandTooFarFromTrainPowerSource = 60,
  CantLandOnDenseTerrain = 61,
  AddOnTooFarFromBuilding = 62,
  MustBuildRefineryFirst = 63,
  BuildingIsUnderConstruction = 64,
  CantFindDropOff = 65,
  CantLoadOtherPlayersUnits = 66,
  NotEnoughRoomToLoadUnit = 67,
  CantUnloadUnitsThere = 68,
  CantWarpInUnitsThere = 69,
  CantLoadImmobileUnits = 70,
  CantRechargeImmobileUnits = 71,
  CantRechargeUnderConstructionUnits = 72,
  CantLoadThatUnit = 73,
  NoCargoToUnload = 74,
  LoadAllNoTargetsFound = 75,
  NotWhileOccupied = 76,
  CantAttackWithoutAmmo = 77,
  CantHoldAnyMoreAmmo = 78,
  TechRequirementsNotMet = 79,
  MustLockdownUnitFirst = 80,
  MustTargetUnit = 81,
  MustTargetInventory = 82,
  MustTargetVisibleUnit = 83,
  MustTargetVisibleLocation = 84,
  MustTargetWalkableLocation = 85,
  MustTargetPawnableUnit = 86,
  YouCantControlThatUnit = 87,
  YouCantIssueCommandsToThatUnit = 88,
  MustTargetResources = 89,
  RequiresHealTarget = 90,
  RequiresRepairTarget = 91,
  NoItemsToDrop = 92,
  CantHoldAnyMoreItems = 93,
  CantHoldThat = 94,
  TargetHasNoInventory = 95,
  CantDropThisItem = 96,
  CantMoveThisItem = 97,
  CantPawnThisUnit = 98,
  MustTargetCaster = 99,
  CantTargetCaster = 100,
  MustTargetOuter = 101,
  CantTargetOuter = 102,
  MustTargetYourOwnUnits = 103,
  CantTargetYourOwnUnits = 104,
  MustTargetFriendlyUnits = 105,
  CantTargetFriendlyUnits = 106,
  MustTargetNeutralUnits = 107,
  CantTargetNeutralUnits = 108,
  MustTargetEnemyUnits = 109,
  CantTargetEnemyUnits = 110,
  MustTargetAirUnits = 111,
  CantTargetAirUnits = 112,
  MustTargetGroundUnits = 113,
  CantTargetGroundUnits = 114,
  MustTargetStructures = 115,
  CantTargetStructures = 116,
  MustTargetLightUnits = 117,
  CantTargetLightUnits = 118,
  MustTargetArmoredUnits = 119,
  CantTargetArmoredUnits = 120,
  MustTargetBiologicalUnits = 121,
  CantTargetBiologicalUnits = 122,
  MustTargetHeroicUnits = 123,
  CantTargetHeroicUnits = 124,
  MustTargetRoboticUnits = 125,
  CantTargetRoboticUnits = 126,
  MustTargetMechanicalUnits = 127,
  CantTargetMechanicalUnits = 128,
  MustTargetPsionicUnits = 129,
  CantTargetPsionicUnits = 130,
  MustTargetMassiveUnits = 131,
  CantTargetMassiveUnits = 132,
  MustTargetMissile = 133,
  CantTargetMissile = 134,
  MustTargetWorkerUnits = 135,
  CantTargetWorkerUnits = 136,
  MustTargetEnergyCapableUnits = 137,
  CantTargetEnergyCapableUnits = 138,
  MustTargetShieldCapableUnits = 139,
  CantTargetShieldCapableUnits = 140,
  MustTargetFlyers = 141,
  CantTargetFlyers = 142,
  MustTargetBuriedUnits = 143,
  CantTargetBuriedUnits = 144,
  MustTargetCloakedUnits = 145,
  CantTargetCloakedUnits = 146,
  MustTargetUnitsInAStasisField = 147,
  CantTargetUnitsInAStasisField = 148,
  MustTargetUnderConstructionUnits = 149,
  CantTargetUnderConstructionUnits = 150,
  MustTargetDeadUnits = 151,
  CantTargetDeadUnits = 152,
  MustTargetRevivableUnits = 153,
  CantTargetRevivableUnits = 154,
  MustTargetHiddenUnits = 155,
  CantTargetHiddenUnits = 156,
  CantRechargeOtherPlayersUnits = 157,
  MustTargetHallucinations = 158,
  CantTargetHallucinations = 159,
  MustTargetInvulnerableUnits = 160,
  CantTargetInvulnerableUnits = 161,
  MustTargetDetectedUnits = 162,
  CantTargetDetectedUnits = 163,
  CantTargetUnitWithEnergy = 164,
  CantTargetUnitWithShields = 165,
  MustTargetUncommandableUnits = 166,
  CantTargetUncommandableUnits = 167,
  MustTargetPreventDefeatUnits = 168,
  CantTargetPreventDefeatUnits = 169,
  MustTargetPreventRevealUnits = 170,
  CantTargetPreventRevealUnits = 171,
  MustTargetPassiveUnits = 172,
  CantTargetPassiveUnits = 173,
  MustTargetStunnedUnits = 174,
  CantTargetStunnedUnits = 175,
  MustTargetSummonedUnits = 176,
  CantTargetSummonedUnits = 177,
  MustTargetUser1 = 178,
  CantTargetUser1 = 179,
  MustTargetUnstoppableUnits = 180,
  CantTargetUnstoppableUnits = 181,
  MustTargetResistantUnits = 182,
  CantTargetResistantUnits = 183,
  MustTargetDazedUnits = 184,
  CantTargetDazedUnits = 185,
  CantLockdown = 186,
  CantMindControl = 187,
  MustTargetDestructibles = 188,
  CantTargetDestructibles = 189,
  MustTargetItems = 190,
  CantTargetItems = 191,
  NoCalldownAvailable = 192,
  WaypointListFull = 193,
  MustTargetRace = 194,
  CantTargetRace = 195,
  MustTargetSimilarUnits = 196,
  CantTargetSimilarUnits = 197,
  CantFindEnoughTargets = 198,
  AlreadySpawningLarva = 199,
  CantTargetExhaustedResources = 200,
  CantUseMinimap = 201,
  CantUseInfoPanel = 202,
  OrderQueueIsFull = 203,
  CantHarvestThatResource = 204,
  HarvestersNotRequired = 205,
  AlreadyTargeted = 206,
  CantAttackWeaponsDisabled = 207,
  CouldntReachTarget = 208,
  TargetIsOutOfRange = 209,
  TargetIsTooClose = 210,
  TargetIsOutOfArc = 211,
  CantFindTeleportLocation = 212,
  InvalidItemClass = 213,
  CantFindCancelOrder = 214
};
bool ActionResult_IsValid(int value);
const ActionResult ActionResult_MIN = Success;
const ActionResult ActionResult_MAX = CantFindCancelOrder;
const int ActionResult_ARRAYSIZE = ActionResult_MAX + 1;

const ::google::protobuf::EnumDescriptor* ActionResult_descriptor();
inline const ::std::string& ActionResult_Name(ActionResult value) {
  return ::google::protobuf::internal::NameOfEnum(
    ActionResult_descriptor(), value);
}
inline bool ActionResult_Parse(
    const ::std::string& name, ActionResult* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ActionResult>(
    ActionResult_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace SCRAPIProtocol

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::SCRAPIProtocol::ActionResult> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::SCRAPIProtocol::ActionResult>() {
  return ::SCRAPIProtocol::ActionResult_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_error_2eproto
