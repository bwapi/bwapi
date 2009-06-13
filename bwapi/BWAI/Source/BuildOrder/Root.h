#pragma once

#include <list>
#include <map>
#include <set>
#include <string>

namespace BuildOrder { class Branch; }
namespace BuildOrder { class RootBranch; }
namespace BuildOrder { class BuildWeights; }
namespace BuildOrder { class Executor; }
namespace Util { class Logger; }

namespace BuildOrder
{
  /** Holder of all build order branches and build-order specific settings. */
  class Root
  {
    public :
      Root(const std::string& xmlFileName);
      void include(const std::string& xmlFileName);
      ~Root();
      std::map<std::string, Branch*> functions;
      std::list<RootBranch*> buildOrders;
      std::map<std::string, BuildWeights*> weights;
      Util::Logger *log;
      Executor* getStart();
      std::set<std::string> includedFiles;
  };
}