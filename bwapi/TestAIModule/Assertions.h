#pragma once
#include <string>
#include <fstream>
#include <deque>

#define TEST(a,x) ((x) ? (a).Pass() : (a).Fail(__FILE__,__LINE__))

class AssertTracker
{
public:
  AssertTracker(const std::string &logFileName = "failedAsserts.log", const std::string &title = "");

  std::ostream &Fail(const char * const file, unsigned line);
  std::ostream &Pass();

  void updateDisplay() const;

  void start() const;

private:
  static unsigned idMax;

  // Members
  std::string name;
  unsigned id;
  unsigned passCount;
  unsigned failCount;
  std::ofstream ofs;

  std::deque<std::string> recent;
};

extern AssertTracker Assert;
