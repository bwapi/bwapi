#pragma once
#include <chrono>

namespace BWAPI
{
  // Keeps track of Actions Per Minute (APM).
  class APMCounter
  {
  public:
    // Reset/initialize all values
    void init();

    // Retrieves the apm value
    int apm(bool selects = true) const;

    // Updates the APM counter
    void update();

    // Increments the APM counter for selections
    void addSelect();

    // Increments the APM counter for non-selections
    void addNoSelect();

  private:
    int     botAPM_noselects = 0;
    int     botAPM_selects = 0;
    long double  botAPMCounter_noselects = 0;
    long double  botAPMCounter_selects = 0;

    int lastUpdateFrame = 0;
  };

}
