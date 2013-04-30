/**
 * @file MbusMasterFunctions.hpp
 *
 * @if NOTICE
 *
 * Copyright (c) 2002-2009 proconX Pty Ltd. All rights reserved. 
 *
 * THIS IS PROPRIETARY SOFTWARE AND YOU NEED A LICENSE TO USE OR REDISTRIBUTE.
 * 
 * THIS SOFTWARE IS PROVIDED BY PROCONX AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL PROCONX OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @endif
 */


#ifndef _MBUSMASTERFUNCTIONS_H_INCLUDED
#define _MBUSMASTERFUNCTIONS_H_INCLUDED

#ifndef __cplusplus
#  error Must use C++ to compile this module!
#endif

// Platform header
#include <stdlib.h>

// Package header
#include "BusProtocolErrors.h"


/*****************************************************************************
 * Forward declaration
 *****************************************************************************/

class HmTimer;


/*****************************************************************************
 * MbusMasterFunctions class declaration
 *****************************************************************************/

/**
 * @brief Base class which implements Modbus data and control functions
 *
 * The functions provided by this base class apply to all protocol flavours
 * via inheritance. For a more detailed description see section @ref
 * mbusmaster.
 *
 * @see mbusmaster
 * @see MbusSerialMasterProtocol, MbusRtuMasterProtocol
 * @see MbusAsciiMasterProtocol, MbusTcpMasterProtocol
 */
class
#if defined (_WINDLL) || defined(__DLL__)
   __declspec(dllexport)
#endif
MbusMasterFunctions
{

  protected:

   enum
   {
      PDU_SIZE = 253, // As per MODBUS APPLICATION PROTOCOL SPECIFICATION V1.1a
      MAX_DATA_SIZE = PDU_SIZE - 1, // = 252 = PDU minus 1 byte function code
      
      MAX_FC03_WORDS = (MAX_DATA_SIZE - 1) / 2, // =  125 = maximum words per read request
      //MAX_FC04_WORDS =  MAX_FC03_WORDS,       // same as function code 03
      MAX_FC01_COILS =  MAX_FC03_WORDS * 16,    // = 2000 = maximum coils per read request
      //MAX_FC02_COILS =  MAX_FC01_COILS,       // same as function code 1
      MAX_FC16_WORDS = (MAX_DATA_SIZE - 5) / 2, // =  123 = maximum words per write request
      MAX_FC15_COILS =  MAX_FC16_WORDS * 16,    // = 1968 = maximum coils per write request
      MAX_FC23_READ  =  MAX_FC03_WORDS,         // =  125 = maximum words for read
      MAX_FC23_WRITE = (MAX_DATA_SIZE - 9) / 2  // =  121 = maximum words for write
   };

   volatile unsigned long totalCounter;
   volatile unsigned long successCounter;
   int retryCnt;  // Retry counter
   int timeOut;   // Time-out in ms
   int pollDelay; // Delay between two Modbus read/writes in ms
   HmTimer &silenceTimer;


  private:

   char bufferArr [MAX_DATA_SIZE];
   int bigEndianMachine;      // Auto-configured by constructor
   int bigEndianFloatMachine; // Auto-configured by constructor
   int swapInts;              // User flags
   int swapFloats;            // User flags

   //
   // Slave configuration data
   // 
   int slaveConfigFlagsArr[256];


  protected:

    MbusMasterFunctions();


  public:

   virtual ~MbusMasterFunctions();


   /**************************************************************************
    * Modbus function codes
    **************************************************************************/

  public:

   /**
    * @name Bit Access
    * Table 0:00000 (Coils) and Table 1:0000 (Input Status)
    * @ingroup mbusmaster
    */
   //@{

   int readCoils(int slaveAddr, int startRef,
                 int bitArr[], int refCnt);

   int readInputDiscretes(int slaveAddr, int startRef,
                          int bitArr[], int refCnt);

   int writeCoil(int slaveAddr, int bitAddr, int bitVal);

   int forceMultipleCoils(int slaveAddr, int startRef,
                          const int bitArr[], int refCnt);

   //@}


   /**
    * @name 16-bit Access
    *  Table 4:00000 (Holding Registers) and Table 3:00000 (Input Registers)
    * @ingroup mbusmaster
    */
   //@{

   int readMultipleRegisters(int slaveAddr,
                             int startRef,
                             short regArr[], int refCnt);

   int readInputRegisters(int slaveAddr, int startRef,
                          short regArr[], int refCnt);

   int writeSingleRegister(int slaveAddr, int regAddr, short regVal);

   int writeMultipleRegisters(int slaveAddr,
                              int startRef,
                              const short regArr[], int refCnt);

   int maskWriteRegister(int slaveAddr, int regAddr,
                         short andMask, short orMask);

   int readWriteRegisters(int slaveAddr,
                          int readRef, short readArr[], int readCnt,
                          int writeRef,
                          const short writeArr[], int writeCnt);
   //@}

   
   /**
    * @name 32-bit Access
    *  Table 4:00000 (Holding Registers) and Table 3:00000 (Input Registers)
    * @ingroup mbusmaster
    */
   //@{

   int readMultipleLongInts(int slaveAddr,
                            int startRef,
                            long int32Arr[], int refCnt);

   int readInputLongInts(int slaveAddr,
                         int startRef,
                         long int32Arr[], int refCnt);

   int writeMultipleLongInts(int slaveAddr,
                             int startRef,
                             const long int32Arr[], int refCnt);

   int readMultipleFloats(int slaveAddr,
                          int startRef,
                          float float32Arr[], int refCnt);

   int readInputFloats(int slaveAddr,
                       int startRef,
                       float float32Arr[], int refCnt);

   int writeMultipleFloats(int slaveAddr,
                           int startRef,
                           const float float32Arr[], int refCnt);

   int readMultipleMod10000(int slaveAddr,
                            int startRef,
                            long int32Arr[], int refCnt);

   int readInputMod10000(int slaveAddr,
                         int startRef,
                         long int32Arr[], int refCnt);

   int writeMultipleMod10000(int slaveAddr,
                             int startRef,
                             const long int32Arr[], int refCnt);

   //@}


   /**
    * @name Diagnostics
    * @ingroup mbusmaster
    */
   //@{

   int readExceptionStatus(int slaveAddr, unsigned char *statusBytePtr);

   int returnQueryData(int slaveAddr,
                       const unsigned char queryArr[],
                       unsigned char echoArr[], int len);

   int restartCommunicationsOption(int slaveAddr, 
                                   int clearEventLog);
   
   //@}


   /**
    * @name Custom Function Codes
    * @ingroup devicespecific 
    */
   //@{

   int customFunction(int slaveAddr, 
                      int functionCode,
                      void* requestData, size_t requestLen,
                      void* responseData, size_t *responseLenPtr);

   //@}

   
   /**************************************************************************
    * Protocol Configuration
    **************************************************************************/

   /**
    * @name Protocol Configuration
    * @ingroup mbusmaster
    */
   //@{

  public:

    int setTimeout(int timeOut);

   /**
    * Returns the time-out value.
    *
    * @remark The time-out value is indicative only and not guaranteed to
    * be maintained. How precise it is followed depends on the operating
    * system used, it's scheduling priority and it's system timer
    * resolution.
    * @return Timeout value in ms
    */
   int getTimeout() { return (timeOut); }

   int setPollDelay(int pollDelay);

   /**
    * Returns the poll delay time.
    *
    * @return Delay time in ms, 0 if poll delay is switched off
    */
   int getPollDelay() { return (pollDelay); }

   int setRetryCnt(int retryCnt);

   /**
    * Returns the automatic retry count.
    *
    * @return Retry count
    */
   int getRetryCnt() { return (retryCnt); }

   //@}


   /**************************************************************************
    * Transmission Statistic Functions
    **************************************************************************/

   /**
    * @name Transmission Statistic Functions
    * @ingroup mbusmaster
    */
   //@{

  public:

   /**
    * Returns how often a message transfer has been executed.
    *
    * @return Counter value
    */
   long getTotalCounter() { return (totalCounter); }

   /**
    * Resets total message transfer counter.
    */
   void resetTotalCounter() { totalCounter = 0UL; }

   /**
    * Returns how often a message transfer was successful.
    *
    * @return Counter value
    */
   long getSuccessCounter() { return (successCounter); }

   /**
    * Resets successful message transfer counter.
    */
   void resetSuccessCounter() { successCounter = 0UL; }

   //@}


   /**************************************************************************
    * Slave Configuration
    **************************************************************************/

  public:

   /**
    * @name Slave Configuration
    * @ingroup mbusmaster
    */
   //@{

   void configureBigEndianInts();

   void configureLittleEndianInts();

   void configureIeeeFloats();

   void configureSwappedFloats();

   void configureStandard32BitMode();

   void configureEnron32BitMode();
   
   void configureCountFromOne();
   
   void configureCountFromZero();

   int configureBigEndianInts(int slaveAddr);

   int configureLittleEndianInts(int slaveAddr);

   int configureIeeeFloats(int slaveAddr);

   int configureSwappedFloats(int slaveAddr);
   
   int configureStandard32BitMode(int slaveAddr);
   
   int configureEnron32BitMode(int slaveAddr);

   int configureCountFromOne(int slaveAddr);

   int configureCountFromZero(int slaveAddr);
   
   //@}


   /**************************************************************************
    * Utility routines
    **************************************************************************/

  public:

   /**
    * Returns whether the protocol is open or not.
    *
    * @retval true = open
    * @retval false = closed
    */
   virtual int isOpen() = 0;

   /**
    * Closes an open protocol including any associated communication
    * resources (com ports or sockets).
    */
   virtual void closeProtocol() = 0;

   static TCHAR *getPackageVersion();


   /**************************************************************************
    * Internal subroutines
    **************************************************************************/

  protected:

   virtual int deliverMessage(int address, int function,
                              char sendDataArr[], int sendDataLen,
                              char rcvDataArr[], int rcvDataLen, 
                              int *actualRcvdPtr = NULL) = 0;

  private:

   int readBits(int function, int slaveAddr, int startRef,
                int bitArr[], int refCnt);

   int readRegisters(int function, int slaveAddr, int startRef,
                     short regArr[], int refArrLen, int regCnt);

   int writeRegisters(int slaveAddr, int startRef, 
                      const short regArr[], int refCnt, int regCount);

  private:

   // Disable default operator and copy constructor
   MbusMasterFunctions &operator= (MbusMasterFunctions &);
   MbusMasterFunctions (const MbusMasterFunctions &);

};


#endif // ifdef ..._H_INCLUDED

