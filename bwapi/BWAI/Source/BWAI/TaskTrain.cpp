#include "TaskTrain.h"

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskTrain::TaskTrain(BW::UnitType buildingType, BuildOrder::BuildWeights* weights)
  :Task()
  ,buildingType(buildingType)
  ,weights(weights)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskTrain::~TaskTrain()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskTrain::execute()
  {
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskTrain::getType()
  {
    return TaskType::Train;
  }
  //------------------------------- GET MINERAL -------------------------------
  BW::UnitType TaskTrain::getBuildingType()
  {
    return this->buildingType;
  }
  //---------------------------------------------------------------------------
  BWAPI::ReservedResources TaskTrain::getReserved()
  {
    return BWAPI::ReservedResources();
  }
  //---------------------------------------------------------------------------
}