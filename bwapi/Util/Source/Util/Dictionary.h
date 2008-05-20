#ifndef CDICTIONARYH
#define CDICTIONARYH
#include "Sentence.h"
#include "DictionaryFile.h"
#include <list>
#include <map>
#include <string>
#include <vector>
class Language;
class MultiString;
/**
 * Utility for work with property files.
 * Dictionary represents content of set of property files with constant(length of word) acces to it's values.
 * Dictionary consists of two different data structures, the first one is hash table of all values,
 * the second one is list of files (DictionaryFile) containing list of lines (DictionaryLine).
 * The first strcture enables fast acces property values, while the second one enables to change 
 * some property values and save these changes back to it's dictionary files while position of every 
 * property value will be unchanged same as all comemnts, this is practical mainly for configuration 
 * property files.<br>
 * Every value of the dicionary is represnetd by class Sentence, and the same sentence is referenced
 * both from DictionaryLine and the hash table.
 * <br><br>
 * property file syntax: <br>
 * key=value, parameter 1 is {1}, parameter 3 is {3}, value of hello is {hello}
 * @see Sentence
 */
class Dictionary
 {
  private :
    /** Name of the dictionary, typicaly same as the name of the language */
    std::string name;
	   /** 
	    * Source property files of the dictionary
	    * @see DictionaryFile
	    */
    std::list<DictionaryFile*> files;
  public :
    std::map<std::string, Sentence*> values;
    /** 
	    * Creates dictionary from single source property file
	    * @param fileName Property file location.
	    */
     Dictionary(const std::string &fileName);
	    /** Removes dictionary and all contained keys and dictionaryFiles (not files in file system, just their representation in memory) */
     virtual ~Dictionary();
	   /** 
	    * Gets value of the specified key.
	    * If the dictionary contains more values with the same key, one of these values will be (pseudorandomly) choosen.
	    * @param key Key of the property value to be returned.
	    * @return Value of property with the specified key if such value exists, otherwise string 
	    * '{Key '[key]' not found }' will be returned.
	    */
     std::string getValue(const std::string &key);
	   /** 
	    * Gets value of the specified key.
	    * If the dictionary contains more values with the same key, one of these values will be (pseudorandomly) choosen.
	    * @param key Key of the property value to be returned.
        * @param arguments Arguments of the property value, first argument corresponds to {1} etc.
	    * @return Value of property with the specified key if such value exists, otherwise string 
	    * '{Key '[key]' not found }' will be returned.
	    */
     std::string getValue(const std::string &key, std::vector<std::string>* arguments);
     std::string getValue(const std::string &key, const std::string &parameter1);
     std::string getValue(const std::string &key, const std::string &parameter1, const std::string &parameter2);
     std::string getValue(const std::string &key, const std::string &parameter1, const std::string &parameter2, const std::string &parameter3);
     std::string getValue(const std::string &key, const std::string &parameter1, const std::string &parameter2, const std::string &parameter3, const std::string &parameter4);
     std::string getValue(const std::string &key, const std::string &parameter1, const std::string &parameter2, const std::string &parameter3, const std::string &parameter4, const std::string &parameter5);
     std::string getValue(const std::string &key, const std::string &parameter1, const std::string &parameter2, const std::string &parameter3, const std::string &parameter4, const std::string &parameter5, const std::string &parameter6);
     /** 
	     * Gets value of the specified key.
	     * @param key Key of the property value to be returned.
	     * @return Value of property with the specified key and given arguments, otherwise param key will be returned.
	     */
     std::string getValueSafe(const std::string &key);
     /** 
   	  * Reloads whole dictionary for the specified language.
	     * @param language New language pack to be loaded.
      */
     void reload(Language* language);
     std::string getName(void);
     void setName(const std::string &name);
     void saveToFile(void);
   private :
     void load(const std::string &fileName);
     void clearAll(void);
 };
#endif
