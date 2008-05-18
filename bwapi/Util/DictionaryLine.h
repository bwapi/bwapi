#ifndef DICTIONARYLINEH
#define DICTIONARYLINEH

#include <string>
/** 
 * Represents one line in property file.
 * It can be line containg property value (Sentence) or otherwise (UnusedLine)
 * @see Dictionary
 */
class DictionaryLine
 {
  public :
   virtual ~DictionaryLine();
   virtual std::string getLine(void) = 0;
 };
#endif
