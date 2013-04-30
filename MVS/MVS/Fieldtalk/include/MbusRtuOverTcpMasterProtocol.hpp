/**
 * @file MbusRtuOverTcpMasterProtocol.hpp
 *
 * @if NOTICE
 *
 * Copyright (c) 2004-2008 proconX Pty Ltd. All rights reserved. 
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


#ifndef _MBUSRTUOVERMASTERPROTOCOL_H_INCLUDED
#define _MBUSRTUOVERMASTERPROTOCOL_H_INCLUDED

#ifndef __cplusplus
#  error Must use C++ to compile this module!
#endif


// Package header
#include "hmtcpip.h"
#include "MbusMasterFunctions.hpp"
#include "MbusTcpMasterProtocol.hpp"


/*****************************************************************************
 * MbusRtuOverTcpMasterProtocol class declaration
 *****************************************************************************/

/**
 * Encapsulated Modbus RTU Master Protocol class
 *
 * This class realises the Encapsulated Modbus RTU master protocol.
 * This protocol is also known as RTU over TCP or RTU/IP and used for
 * example by ISaGraf<sup>&reg;</sup> Soft-PLCs. This class provides functions to establish
 * and to close a TCP/IP connection to the slave as well as data and control 
 * functions which can be used after a connection to a slave device has been 
 * established successfully. The data and control functions are organized 
 * different conformance classes. For a more detailed description of the data 
 * and control functions see section 
 * @ref mbusmaster.
 *
 * It is also possible to instantiate multiple instances of this class for
 * establishing multiple connections to either the same or different hosts.
 *
 * @ingroup mbusmasterrtuovertcp
 * @see mbusmaster
 * @see MbusMasterFunctions
 */
class
#if defined (_WINDLL) || defined(__DLL__)
   __declspec(dllexport)
#endif
MbusRtuOverTcpMasterProtocol: public MbusTcpMasterProtocol
{
  private:

   enum
   {
      // RTU header length is 2 bytes address/function
      HDR_LEN = 2 ,
      // RTU frame length is header + 2 bytes crc
      FRAME_LEN = HDR_LEN + 2,
      // RTU exception message length is frame + 1 byte exception code
      EXC_MSG_LEN = FRAME_LEN + 1,
      // RTU max. message size is data size + frame length
      MAX_MSG_SIZE = MAX_DATA_SIZE + FRAME_LEN
   };

   char bufferArr[MAX_MSG_SIZE];


  public:

   MbusRtuOverTcpMasterProtocol();

   int openProtocol(const TCHAR * const hostName);

   int setPort(unsigned short portNo);


  private:

   // Not support here (only availabe for the super class MbusTcpMasterProtocol)
   int adamSendReceiveAsciiCmd(const char * const commandSz, char* responseSz);

   int transceiveMessage(int address, int function,
                         char sendDataArr[], int sendDataLen,
                         char rcvDataArr[], int rcvDataLen, int *actualRcvdPtr);


  private:

   // Disable default operator and copy constructor
   MbusRtuOverTcpMasterProtocol &operator= (MbusRtuOverTcpMasterProtocol &);
   MbusRtuOverTcpMasterProtocol (const MbusRtuOverTcpMasterProtocol &);

};


#endif // ifdef ..._H_INCLUDED
