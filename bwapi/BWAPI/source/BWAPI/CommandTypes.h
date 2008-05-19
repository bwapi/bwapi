namespace BWAPI
{
  /** Type of #BWAPI#Command descendant. */
  namespace CommandTypes
  {
    enum Enum
    {
     Train       = 0, /** BWAPI#CommandTrain */
     RightClick  = 1, /** BWAPI#CommandRightClick */
     CancelTrain = 2, /** BWAPI#CommandCancelTrain */
     Build       = 3  /** BWAPI#CommandBuild */
    };
  }
}