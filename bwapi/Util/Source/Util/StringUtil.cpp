#include "StringUtil.h"

#include <math.h>

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
RectangleArray<char> StringUtil::makeBorder(const RectangleArray<char>& input, bool coordinates)
{
  int leftBorder = (int)log10((float)input.getHeight()) + 2;
  int topBorder = 3;
  RectangleArray<char> returnValue = RectangleArray<char>(input.getWidth() + leftBorder*2, input.getHeight() + topBorder*2);
  for (unsigned int x = 0; x < returnValue.getWidth(); x++)
    for (unsigned int y = 0; y < returnValue.getHeight(); y++)  
      returnValue[x][y] = ' ';
  StringUtil::makeWindow(returnValue,
                         leftBorder - 1, 
                         topBorder - 1,
                         input.getWidth() + 2, 
                         input.getHeight() + 2);      
  for (unsigned int x = 0; x < input.getWidth(); x++)
    for (unsigned int y = 0; y < input.getHeight(); y++)  
      returnValue[x + leftBorder ][y + topBorder] = input[x][y];
  for (unsigned int i = 0; i < input.getWidth(); i+=10)
  {
    StringUtil::printTo(returnValue, StringUtil::intToString(i), i + leftBorder, 0);
    StringUtil::printTo(returnValue, StringUtil::intToString(i), i + leftBorder, returnValue.getHeight() - 1);
  }
  for (unsigned int i = 0; i < input.getWidth(); i++)
  {
    StringUtil::printTo(returnValue, StringUtil::intToString(i%10), i + leftBorder, 1);
    StringUtil::printTo(returnValue, StringUtil::intToString(i%10), i + leftBorder, returnValue.getHeight() - 2);
  }
  
  for (unsigned int i = 0; i < input.getHeight(); i++)
  {
    StringUtil::printTo(returnValue, StringUtil::intToString(i), 0 , i + topBorder);
    StringUtil::printTo(returnValue, StringUtil::intToString(i), leftBorder + input.getWidth() + 1, i + topBorder);
  }

  return returnValue;
}
char StringUtil::FrameCharacters[2][6] = 
{
  {
    char(205),
    char(186),
    char(201),
    char(187),
    char(200),
    char(188)
  },
  {
    char(196),
    char(179),
    char(218),
    char(191),
    char(192),
    char(217)
  }
};
 
//------------------------------------------------------------------------------
void StringUtil::makeWindow(RectangleArray<char>& input, unsigned int x, 
                            unsigned int y, 
                            unsigned int width, 
                            unsigned int height, 
                            int frameType)
{
  for (unsigned int i = x + 1; i < x + width - 1 && x < input.getWidth(); i++)
  {
    input[i][y] = StringUtil::FrameCharacters[frameType][0];
    input[i][y + height - 1] = StringUtil::FrameCharacters[frameType][0];
  }
  
  for (unsigned int i = y + 1; i < y + height - 1 && y < input.getHeight(); i++)
  {
    input[x][i] = StringUtil::FrameCharacters[frameType][1];
    input[x + height - 1][i] = StringUtil::FrameCharacters[frameType][1];
  }
  input[x][y] = StringUtil::FrameCharacters[frameType][2];
  input[x + width - 1][y] = StringUtil::FrameCharacters[frameType][3];
  input[x][y + height - 1] = StringUtil::FrameCharacters[frameType][4];
  input[x + width - 1][y + height- 1] = StringUtil::FrameCharacters[frameType][5];
}
//------------------------------------------------------------------------------
void StringUtil::printTo(RectangleArray<char>& input, 
                         const std::string& text, 
                         unsigned int x, 
                         unsigned int y)
{
  for (unsigned int i = 0; text[i] != 0; i++)
    input[x + i][y] = text[i];
}
//------------------------------------------------------------------------------