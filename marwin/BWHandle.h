#include <windows.h>
#include "Exception.h"

class BWHandle
 {
  public :
    /**
     * Constructor
     * Open bw process
     * @throws Exception in case of error
     */
    BWHandle(void);
    /**
     * Destructor
     * Closes the process handle
     */
   ~BWHandle(void);
    /** Reads memory from broodwar
     * @param address Memory address to read from
     * @param size Count of bytes to read
     * @param data Buffer the data will be stored in.
     */
   void readMemory(LPVOID address, void* data, int size);
    /** Writes to broodwar memory
     * @param address Memory address to write to
     * @param size Count of bytes to write
     * @param data Buffer the data that will be written to memory.
     */
   void writeMemory(LPVOID address, void* data, int size);
  private :
    HANDLE hProcess; /**< Handle of the broodwar process */
    HANDLE hToken; /**< Don't know something with the process also. */
    /** Sets Privilege of process
        @param hToken Token of the handle of the process to update
        @param Privilege Privilege to enable/disable
        @param bEnablePrivilege @c true to enable.  @c false to disable
        @return @c true if the operation was succesful, @false otherwise
      */
    bool setPrivilege(HANDLE hToken, LPCTSTR Privilege, bool bEnablePrivilege);
 };