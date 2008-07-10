#include <list>

namespace BWAI { class MapStartingPosition; }
namespace BWAI { class MapExpansion; }

namespace BWAI
{
  /** Additional (pre-prepared) data that the ai uses to operated over the map. */
  class MapInfo
  {
    public :
      /**
       * Loads it's content from the specified xml file, it's structure has no 
       *  formal xsd yet, but it can be guessed from the xml file
       * @param location of the xml file, don't forget that the "." directory is
       *        the 
       *         directory is the starcraft.exe directorry
       * @throws FileException if the specified file was not found.
       */
      MapInfo(const std::string& xmlFileName);
      ~MapInfo();
      std::list<MapExpansion*> expansions;
      std::list<MapStartingPosition*> startingPositions;
      MapExpansion *getExpansion(const std::string& id);
      void saveDefinedBuildingsMap(const std::string& fileName);
      //static void standardBuildingLocations(const std::string& fileName);
    private :
  };
}