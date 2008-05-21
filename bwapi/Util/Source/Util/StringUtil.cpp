#include "StringUtil.h"
#include "Exceptions.h"
//------------------------------- INT TO STRING --------------------------------
std::string StringUtil::intToString(long value)
 {
  if (value == 0)
    return "0";
  std::string returnValue;
  while (value != 0)
   {
    returnValue = (char) (48+(value % 10)) + returnValue;
    value/=10;
   }
  if (value >= 0)
    return returnValue;
  else
    return "-" + returnValue;
 }
//------------------------------- STRING TO INT --------------------------------
unsigned long StringUtil::stringToInt(const std::string &input, const unsigned long begin, const int distance)
 {
  unsigned long returnValue = 0;
  for (unsigned long i = begin; i < distance + begin && i < input.size();i++)
   {
    if (!isdigit(input[i]))
      throw ParseException::ParseException("StringUtil::stringToInt - String " + input + " is not a number.");
    returnValue*=10;
    returnValue += (input[i] - '0');
   }
  return returnValue;
 }
//------------------------------- STRING TO FILE -------------------------------
void StringUtil::stringToFile(const std::string &input,FILE* f)
 {
  size_t length = input.size();
  fwrite(&length,sizeof(unsigned long),1,f);
  fwrite(input.c_str(),length*sizeof(char),1,f);
 }
//-------------------------------- SAVE TO FILE --------------------------------
void StringUtil::saveToFile(const std::string &input,const std::string &fileName)
 {
   FILE *f = fopen(fileName.c_str(), "wt");
   fwrite(input.c_str(),input.length()*sizeof(char),1,f);
   fclose(f);
 }
//------------------------------- LOAD FROM FILE -------------------------------
std::string StringUtil::loadFromFile(FILE* f)
 {
  unsigned long Length;
  fread(&Length,sizeof(unsigned long),1,f);
  char* Buffer = new char[Length + 1];
  Buffer[Length] = 0;
  if (Length != 0)
    fread(Buffer,Length*sizeof(char),1,f);
  std::string ReturnValue = Buffer;;
  delete [] Buffer;
  return ReturnValue;
 }
//----------------------------- BEGINS WIHT NUMBER -----------------------------
bool StringUtil::beginsWithNumber(const std::string &input)
 {
  if (input.length() >= 1)
    return input[0]>='0' && input[0]<= '9';
  return false;
 }
//------------------------------ ENDS WITH NUMBER ------------------------------
bool StringUtil::endsWithNumber(const std::string &input)
 {
  if (input.length() >= 1)
    return input[input.length() - 1]>='0' && input[input.length() - 1] <= '9';
  return false;
 }
//------------------------------- LOAD FROM FILE -------------------------------

void StringUtil::loadFromFile(const std::string &fileName, std::string &target,const long bufferSize)
 {
  char* buffer = new char[bufferSize];
  FILE* f = fopen(fileName.c_str(),"rt");
  size_t fileSize;
  if (f)
   {
    fileSize = fread(buffer,1,bufferSize,f);
    fclose(f);
    buffer[fileSize] = 0;
    target = buffer;
   }
  else
   throw new FileException("Couldn't open file " + fileName);
  delete [] buffer;
 }
//---------------------------------- TRIM ALL ----------------------------------
std::string StringUtil::trimAll(std::string input)
 {
  size_t length = input.size();
  char* buffer = new char[length + 1];
  long pos = 0;
  for (size_t i = 0;i < length;i++)
   {
    if (!isspace(input[i]))
     {
      buffer[pos] = input[i];
      pos++;
     }
   }
  buffer[pos] = 0;
  std::string returnValue = buffer;
  delete [] buffer;
  return returnValue;
 }
//---------------------------------- TRIM ALL ----------------------------------
std::string StringUtil::trim(std::string input)
 {
  size_t i, j;
  for (i = 0; i < input.length() && isspace(input[i]);i++);
  if (i == input.length())
    return  "";
  for (j = input.length() - 1; j > 0 && isspace(input[j]);j--);
  if (i == 0 && j == input.length())
    return input;
  else
    return input.substr(i,j - i + 1);
 }

char StringUtil::buffer[STRING_UTIL_BUFFER_SIZE];
//------------------------------ READ LINE -------------------------------------
std::string StringUtil::readLine(FILE* f)
 {
  std::string result;
  readNextBlock:
  int position = 0;
  fread(buffer, sizeof(char), 1, f);
  while (buffer[position] != 13 && buffer[position] != 10 && position < STRING_UTIL_BUFFER_SIZE - 1 && !feof(f))
   {
    position++;
    fread(&buffer[position], 1, 1,f);
   }

  if (buffer[position] == 13 || buffer[position] == 10 || feof(f))
   {
    buffer[position] = 0;
    result.append(buffer);
   	return result;
   }
  else
   {
    buffer[position + 1] = 0;
    result.append(buffer);
    goto readNextBlock;
   }
 }
//------------------------------------------------------------------------------
const std::string& StringUtil::dereferenceString(const std::string* const input)
 {
  return *input;
 }
//------------------------------------------------------------------------------
