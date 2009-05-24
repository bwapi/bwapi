#include <string>
namespace BWAI
{
  namespace Map
  {
    /**
     * Saves the buildability map for the currently opened map to the
     * specified file.
     */
    std::string saveBuildabilityMap(const std::string& fileName);
    std::string saveWalkabilityMap(const std::string& fileName);
    std::string saveHeightMap(const std::string& fileName);
    std::string saveFogOfWarMap(const std::string& fileName);
  }
}