/**
 * @file MbusMasterCexports.h
 *
 * @if NOTICE
 *
 * Copyright (c) 2004-2009 proconX Pty Ltd. All rights reserved. 
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


#ifndef _MBUSMASTERCEXPORTS_H_INCLUDED
#define _MBUSMASTERCEXPORTS_H_INCLUDED


/* Platform header */
#include <stdlib.h>
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN32_WCE)
#  include <windows.h> /* Required for CE which defined __stdcall in it's own way...*/
#  define STDCALL   __stdcall
#else
#  define STDCALL
#endif
#if defined (_WINDLL) || defined(__DLL__)
#  define EXPORT    __declspec(dllexport)
#else
#  define EXPORT
#endif

/* UNICODE support (MS VC++, Win CE) */
#if defined (_UNICODE) || defined(UNICODE)
#  ifndef _TCHAR_DEFINED
      typedef wchar_t TCHAR;
#     define _TCHAR_DEFINED
#  endif
#else
#  ifndef _TCHAR_DEFINED
      typedef char TCHAR;
#     define _TCHAR_DEFINED
#  endif
#endif

/* Package header */
#include "BusProtocolErrors.h"


/*****************************************************************************
 * Type definitions
 *****************************************************************************/

typedef void * MbusMasterProtocolHdl;


/*****************************************************************************
 * Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * C wrapper functions to create and destroy protocol objects
 *****************************************************************************/

EXPORT MbusMasterProtocolHdl STDCALL mbusMaster_createTcpProtocol(void);


EXPORT MbusMasterProtocolHdl STDCALL mbusMaster_createRtuProtocol(void);


EXPORT MbusMasterProtocolHdl STDCALL mbusMaster_createRtuOverTcpProtocol(void);


EXPORT MbusMasterProtocolHdl STDCALL mbusMaster_createAsciiProtocol(void);


EXPORT void STDCALL mbusMaster_destroyProtocol(MbusMasterProtocolHdl mbusHdl);


/*****************************************************************************
 * C wrapper function for Bit Access Modbus Function Codes
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_readCoils(MbusMasterProtocolHdl mbusHdl,
                     int slaveAddr, int startRef,
                     int bitArr[], int refCnt);


EXPORT int STDCALL
mbusMaster_readInputDiscretes(MbusMasterProtocolHdl mbusHdl,
                              int slaveAddr, int startRef,
                              int bitArr[], int refCnt);


EXPORT int STDCALL
mbusMaster_writeCoil(MbusMasterProtocolHdl mbusHdl,
                     int slaveAddr, int bitAddr, int bitVal);


EXPORT int STDCALL
mbusMaster_forceMultipleCoils(MbusMasterProtocolHdl mbusHdl,
                              int slaveAddr, int startRef,
                              const int bitArr[], int refCnt);


/*****************************************************************************
 * C wrapper functions for 16-bit Access Modbus Function Codes
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_readMultipleRegisters(MbusMasterProtocolHdl mbusHdl,
                                 int slaveAddr,
                                 int startRef,
                                 short regArr[], int refCnt);


EXPORT int STDCALL
mbusMaster_readInputRegisters(MbusMasterProtocolHdl mbusHdl,
                              int slaveAddr, int startRef,
                              short regArr[], int refCnt);


EXPORT int STDCALL
mbusMaster_writeSingleRegister(MbusMasterProtocolHdl mbusHdl,
                               int slaveAddr,
                               int regAddr,
                               short regVal);


EXPORT int STDCALL
mbusMaster_writeMultipleRegisters(MbusMasterProtocolHdl mbusHdl,
                                  int slaveAddr,
                                  int startRef,
                                  const short regArr[],
                                  int refCnt);


EXPORT int STDCALL
mbusMaster_maskWriteRegister(MbusMasterProtocolHdl mbusHdl,
                             int slaveAddr,
                             int regAddr,
                             short andMask,
                             short orMask);


EXPORT int STDCALL
mbusMaster_readWriteRegisters(MbusMasterProtocolHdl mbusHdl,
                              int slaveAddr,
                              int readRef,
                              short readArr[], int readCnt,
                              int writeRef,
                              const short writeArr[],
                              int writeCnt);


/*****************************************************************************
 * C wrapper functions for 32-bit Access Modbus Function Codes
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_readMultipleLongInts(MbusMasterProtocolHdl mbusHdl,
                                int slaveAddr,
                                int startRef,
                                long int32Arr[], int refCnt);


EXPORT int STDCALL
mbusMaster_readInputLongInts(MbusMasterProtocolHdl mbusHdl,
                             int slaveAddr,
                             int startRef,
                             long int32Arr[], int refCnt);


EXPORT int STDCALL
mbusMaster_writeMultipleLongInts(MbusMasterProtocolHdl mbusHdl,
                                 int slaveAddr,
                                 int startRef,
                                 const long int32Arr[],
                                 int refCnt);


EXPORT int STDCALL
mbusMaster_readMultipleFloats(MbusMasterProtocolHdl mbusHdl,
                              int slaveAddr,
                              int startRef,
                              float float32Arr[], int refCnt);


EXPORT int STDCALL
mbusMaster_readInputFloats(MbusMasterProtocolHdl mbusHdl,
                           int slaveAddr,
                           int startRef,
                           float float32Arr[], int refCnt);


EXPORT int STDCALL
mbusMaster_writeMultipleFloats(MbusMasterProtocolHdl mbusHdl,
                               int slaveAddr,
                               int startRef,
                               const float float32Arr[],
                               int refCnt);


EXPORT int STDCALL
mbusMaster_readMultipleMod10000(MbusMasterProtocolHdl mbusHdl,
                                int slaveAddr,
                                int startRef,
                                long int32Arr[], int refCnt);


EXPORT int STDCALL
mbusMaster_readInputMod10000(MbusMasterProtocolHdl mbusHdl,
                             int slaveAddr,
                             int startRef,
                             long int32Arr[], int refCnt);


EXPORT int STDCALL
mbusMaster_writeMultipleMod10000(MbusMasterProtocolHdl mbusHdl,
                                 int slaveAddr,
                                 int startRef,
                                 const long int32Arr[],
                                 int refCnt);


/*****************************************************************************
 * C wrapper function for Diagnostics Function Codes
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_readExceptionStatus(MbusMasterProtocolHdl mbusHdl,
                               int slaveAddr,
                               unsigned char *statusByte);

EXPORT int STDCALL
mbusMaster_returnQueryData(MbusMasterProtocolHdl mbusHdl,
                           int slaveAddr,
                           const unsigned char queryArr[],
                           unsigned char echoArr[], int len);
EXPORT int STDCALL
mbusMaster_restartCommunicationsOption(MbusMasterProtocolHdl mbusHdl,
                                       int slaveAddr,
                                       int clearEventLog);


/*****************************************************************************
 * C wrapper function for User Defined Function Codes (65-72 and 100-110)
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_customFunction(MbusMasterProtocolHdl mbusHdl,
                          int slaveAddr,
                          int functionCode,
                          void* requestData, size_t requestLen,
                          void* responseData, size_t *responseLenPtr);


/*****************************************************************************
 * C wrapper functions for Protocol Configuration
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_setTimeout(MbusMasterProtocolHdl mbusHdl, int timeOut);


EXPORT int STDCALL mbusMaster_getTimeout(MbusMasterProtocolHdl mbusHdl);


EXPORT int STDCALL
mbusMaster_setPollDelay(MbusMasterProtocolHdl mbusHdl, int pollDelay);


EXPORT int STDCALL mbusMaster_getPollDelay(MbusMasterProtocolHdl mbusHdl);


EXPORT int STDCALL
mbusMaster_setRetryCnt(MbusMasterProtocolHdl mbusHdl, int retryCnt);


EXPORT int STDCALL
mbusMaster_getRetryCnt(MbusMasterProtocolHdl mbusHdl);


/*****************************************************************************
* C wrapper functions for Transmission Statistic Functions
*****************************************************************************/

EXPORT long STDCALL
mbusMaster_getTotalCounter(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_resetTotalCounter(MbusMasterProtocolHdl mbusHdl);


EXPORT long STDCALL
mbusMaster_getSuccessCounter(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_resetSuccessCounter(MbusMasterProtocolHdl mbusHdl);


/*****************************************************************************
 * C wrapper functions for Slave Configuration
 *****************************************************************************/

EXPORT void STDCALL
mbusMaster_configureBigEndianInts(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_configureLittleEndianInts(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_configureIeeeFloats(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_configureSwappedFloats(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_configureStandard32BitMode(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_configureSingleReg32BitMode(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_configureCountFromOne(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL
mbusMaster_configureCountFromZero(MbusMasterProtocolHdl mbusHdl);

/*                                                                            *
 * Per slave functions
 */

EXPORT int STDCALL
mbusMaster_configSlaveBigEndianInts(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


EXPORT int STDCALL
mbusMaster_configSlaveLittleEndianInts(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


EXPORT int STDCALL
mbusMaster_configSlaveIeeeFloats(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


EXPORT int STDCALL
mbusMaster_configSlaveSwappedFloats(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


EXPORT int STDCALL
mbusMaster_configSlaveStandard32BitMode(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


EXPORT int STDCALL
mbusMaster_configEnron32BitMode(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


EXPORT int STDCALL
mbusMaster_configSlaveCountFromOne(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


EXPORT int STDCALL
mbusMaster_configSlaveCountFromZero(MbusMasterProtocolHdl mbusHdl, int slaveAddr);


/*****************************************************************************
 * C wrapper functions for Utility routines
 *****************************************************************************/

EXPORT int STDCALL mbusMaster_isOpen(MbusMasterProtocolHdl mbusHdl);


EXPORT void STDCALL mbusMaster_closeProtocol(MbusMasterProtocolHdl mbusHdl);


EXPORT TCHAR * STDCALL mbusMaster_getPackageVersion(void);


EXPORT TCHAR * STDCALL mbusMaster_getErrorText(int errCode);


/*****************************************************************************
 * C wrapper functions for MbusTcpMasterProtocol class
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_openTcpProtocol(MbusMasterProtocolHdl mbusHdl, const TCHAR * const hostName);


EXPORT int STDCALL
mbusMaster_setTcpPort(MbusMasterProtocolHdl mbusHdl, unsigned short portNo);


EXPORT unsigned short  STDCALL
mbusMaster_getTcpPort(MbusMasterProtocolHdl mbusHdl);

EXPORT int STDCALL
mbusMaster_adamSendReceiveAsciiCmd(MbusMasterProtocolHdl mbusHdl,
                                      const char * const commandSz,
                                      char* responseSz);

/*****************************************************************************
 * C wrapper functions for MbusSerialMasterProtocol class
 *****************************************************************************/

EXPORT int STDCALL
mbusMaster_openSerialProtocol(MbusMasterProtocolHdl mbusHdl,
                               const TCHAR * const portName,
                               long baudRate, int dataBits,
                               int stopBits, int parity);


EXPORT int STDCALL
mbusMaster_enableRs485Mode(MbusMasterProtocolHdl mbusHdl, int rtsDelay);


#ifdef __cplusplus
}
#endif

#endif /* ifdef ..._H_INCLUDED */

