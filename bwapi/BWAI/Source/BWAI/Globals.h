#pragma once

#include "AI.h"

namespace BWAI
{
  /**
   * ai is, and always should be the ONLY instance of the AI class, it is singleton. As there is only one
   * instance, the class is defined globally but it can't be defined as value (as the Broodwar instance is)
   * but have to be pointer, the reason is, that ai uses loaded configuration in the Broodwar instance, so
   * Broodwar have to be constructed before ai, and c++ doesn't allow to specify the order of construction of
   * globall variables, so this class is constructed in DllMain. We wanted to save passing the ai parameter
   * everywhere, so we expect everywhere in the code that this variable is instantialised.
   * @todo we could solve the deletation of this class at the end of the program, but it is really low
   *       priority task.
   */
  extern AI* ai;
}