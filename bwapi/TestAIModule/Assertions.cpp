#include "Assertions.h"
#include <BWAPI.h>
#include <iostream>
using namespace BWAPI;

unsigned AssertTracker::idMax = 0;

AssertTracker Assert("failedAsserts.log", "Main Track");

AssertTracker::AssertTracker(const std::string &logFileName, const std::string &title)
  : id(idMax++)
  , name(title)
  , passCount(0)
  , failCount(0)
  , ofs(std::string("Errors/") + logFileName)
  , recent(10, "-")
{}

std::ostream &AssertTracker::Fail(const char * const file, unsigned line)
{
  // Log file/line
  std::stringstream ss;
  ss << file << ':' << line;
  if ( Broodwar->getLastError() != Errors::None )
    ss << "   LastError: " << Broodwar->getLastError();
  ss << std::endl;

  // Queue the message
  recent.pop_back();
  recent.push_front(ss.str());

  // Increment counter and return the file stream
  ++failCount;
  return ofs << ss;
}

void AssertTracker::updateDisplay() const
{
  Broodwar->setTextSize(Text::Size::Small);

  int x = id * 100;
  int y = 2;
  Broodwar->drawTextScreen(x+2, y, "%c%s\n Succeeded: %u\n Failed: %u", Text::White, name.c_str(), passCount, failCount);
  y += 32;
  for ( auto it = recent.cbegin(); it != recent.cend(); ++it )
  {
    Broodwar->drawTextScreen(x+6, y, "%c%s", Text::Green, it->c_str());
    y += 8;
  }
  Broodwar->setTextSize();
}

void AssertTracker::start() const
{
  Broodwar->registerEvent([&](Game*){ this->updateDisplay(); });
}

std::ostream &AssertTracker::Pass()
{
  ++passCount;
  return std::clog;
}
