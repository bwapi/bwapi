#include "BWAssert.h"
#include <fstream>
#include <stdarg.h>
#include <sys/stat.h>
#include <BWAPI.h>
char buffer[1024];
void log(const char* text, ...)
{
  const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];

  va_list ap;
  va_start(ap, text);
  vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
  va_end(ap);

  FILE *outfile;
  BWAPI::Broodwar->printf(buffer);
  if (fopen_s(&outfile, "bwapi-data/logs/TestModule - Failed Asserts.log", "a+")==0)
  {
    fprintf_s(outfile, buffer);
    fprintf_s(outfile, "\n");
    fclose(outfile);
  }
}