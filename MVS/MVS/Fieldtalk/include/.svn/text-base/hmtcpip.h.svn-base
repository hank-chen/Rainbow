/**
 * @file hmtcpip.h
 *
 * File to include TCP/IP related header files platform independent
 *
 * @if NOTICE
 *
 * $Id: hmtcpip.h,v 1.21 2009/03/31 01:44:42 henrik Exp $
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


#ifndef _HMTCPIP_H_INCLUDED
#define _HMTCPIP_H_INCLUDED


/* HM platform detection */
#include "hmplatf.h"

/* Include TCP/IP related header files */
#if defined(__WIN32__)
#  if defined(UNDER_RTSS) /* RTX */
#    include <winsock2.h>
#  else
#    include <winsock.h>
#  endif
#  include <windows.h>
#elif defined(__VXWORKS__)
#  include <vxWorks.h>
#  include <sockLib.h>
#  include <ioLib.h>
#  include <netinet/tcp.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#elif defined(__UNIX__)
#  include <unistd.h>
#  include <time.h>
#  include <sys/time.h>
#  include <sys/select.h>
#  include <sys/socket.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <netinet/tcp.h>
#  include <netinet/in.h>
#  include <errno.h>
#  include <sys/ioctl.h> /* Should contain FIONBIO */
#  if !defined(FIONBIO) /* Solaris and other UNIXes defines it here instead */
#  include <sys/filio.h>
#  endif
#else
#  error "Platform not yet supported. Please add a new condition to this #if block or update hmplatf.h"
#endif


/*****************************************************************************
 * Some definitions which are not available on every platform
 *****************************************************************************/

#ifndef MSG_NOSIGNAL /* Available on Linux only */
#  define MSG_NOSIGNAL 0
#endif

#ifndef INVALID_SOCKET
#  define INVALID_SOCKET ((SOCKET)(~0))
#endif

#ifndef SOCKET_ERROR
#  define SOCKET_ERROR (-1)
#endif

#ifndef SD_BOTH
#  define SD_BOTH 2
#endif

#if defined(__socklen_t_defined) || defined(socklen_t) || defined(HAVE_SOCKLEN_T) || defined(_SOCKLEN_T_DECLARED)
   typedef socklen_t SOCK_LEN_TYPE;
#elif defined(HAVE_SOCKLEN_U)
   typedef unsigned int SOCK_LEN_TYPE;
#else
   typedef int SOCK_LEN_TYPE;
#endif

#if !defined(_WINSOCKAPI_)
   typedef int SOCKET;
#endif

#if defined(__SOLARIS__) && !defined(INADDR_NONE)
#  define INADDR_NONE (-1)
#endif

#if defined(_WINSOCKAPI_)
#  define SOCKET_EWOULDBLOCK WSAEWOULDBLOCK
#  define SOCKET_EINPROGRESS WSAEINPROGRESS
#  define SOCKET_EINTR WSAEINTR
#  define socketErrno WSAGetLastError()
#else
#  define SOCKET_EWOULDBLOCK EWOULDBLOCK
#  define SOCKET_EINPROGRESS EINPROGRESS
#  define SOCKET_EINTR EINTR
#  define ioctlsocket ioctl
#  define closesocket close
#  define socketErrno errno
#endif


/* UNICODE support (MS VC++, Win CE) */
#if defined(_UNICODE)
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


#endif /* ifdef ..._H_INCLUDED */
