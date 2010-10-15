#pragma once
#include <string>
class MultiString;
/**
 * Represents any exception that can be thrown during to program run,
 * it doesn't contain expressions that can be thrown by tinyXml
 */
class GeneralException
{
  private :
    /**
     * Represents the property key of the exception message, but in some special
     *   cases, the messageKey can contain the message itself
     */
   std::string message;
  public:
    /** Creates exception with the specified message */
    GeneralException(const std::string &messageKey);
    const std::string getMessage(void);
};

/**
 * Can be thrown when the configuration files are invalid, or contain invalid
 * information.
 */
class ConfigException : public GeneralException
{
  public :
    ConfigException(const std::string &message);
};

/** Can be thrown when some required files are not found. */
class FileException : public GeneralException
{
  public :
   FileException(const std::string &message);
};

/** Can be thrown when the xml file structure is invalid. */
class XmlException : public GeneralException
{
    std::string fileName;
    long lineNumber;
  public :
    XmlException(const std::string& message, const std::string& fileName = "", const long lineNumber = 0);
};

/** Can be thrown during parsin (non integer paremters that should be numbers for example) */
class ParseException : public GeneralException
{
  public :
    ParseException(const std::string& message);
};
