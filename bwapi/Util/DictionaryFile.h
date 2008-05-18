#pragma once
#include <list>
#include "DictionaryLine.h"
class Dictionary;

/** Represents one property file. */
class DictionaryFile
{
  private :
    std::string fileName; /**< Name of the file */
    /**
     * Specifies if the file representation has changed sinc it was loaded
     * (used to determine if the file needs to be updated when the file should
     * be saved.)
     */
    bool changed;
  public :
    /** 
     * Loads and parses the given dictionary file.
     * @param fileName Name of file to be loaded as property file.
     * @param dictionary Dictionary containing this file, used to store property values
     *                   into the dictionary and resolve '{key}' statements in property value
     */
    DictionaryFile(const std::string &fileName, Dictionary *dictionary);
    /** Removes all contained lines */
   ~DictionaryFile(void);
    /**
     * Saves the file representation back to it's source file, the source file will change only if
     * some of it's property values has been changed during the program run.
     */
    void saveToFile(void);
    /**
     * Sets if the dictionary file was changed
     */
    void setChanged(const bool changed);
    std::list<DictionaryLine*> lines; /**< Lines of the file*/
};
