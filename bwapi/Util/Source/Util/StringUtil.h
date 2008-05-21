#pragma once
#define STRING_UTIL_BUFFER_SIZE 100
#include <string>

class MultiString;
/** Collection of std::string utilities */
class StringUtil
 {
  private :
   /** The class is abstract, so it has private constructor */
   StringUtil(void);
   static char buffer[STRING_UTIL_BUFFER_SIZE];
  public :
  /**
   * Gets textual representation of the specified number.
   * @param value Number to be converted to std::string.
   * @return Textual representation of the specified number.
   */
  static std::string intToString(long value);
  /**
   * Converts textual representation of number to number.
   * @param input String containing number representation.
   * @param begin Position of the first caracter of the number in the input std::string.
   * @param distance Maximum count of characters to read.
   * @return Returns textual representation of the specified std::string.
   * @throws ParseException if the text can't be converted to integer (non-numerical characters)
   */
  static unsigned long stringToInt(const std::string &input, const unsigned long begin = 0, const int distance = 9);
  static void stringToFile(const std::string &input, FILE* f);
  static void saveToFile(const std::string &input, const std::string &fileName);
  static bool beginsWithNumber(const std::string &input);
  static bool endsWithNumber(const std::string &input);
  static std::string loadFromFile(FILE* f);
  static void loadFromFile(const std::string &fileName,std::string &Target,const long bufferSize);
  static std::string UTF8ToWindows1250(const std::string &input);
  static std::string Windows1250ToUTF8(const std::string &input);
  static std::string trimAll(std::string input);
  static std::string trim(std::string input);
  static std::string replace(const std::string &input, MultiString* values, const std::string &replacement);

  /** 
   * Reads one line from the input stream.
   * @param f Input stream.
   * @return Content of the line.
   */
  static std::string readLine(FILE* f);
  static const std::string& dereferenceString(const std::string* const input);
 };
