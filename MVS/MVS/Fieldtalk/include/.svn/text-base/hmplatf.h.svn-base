/**
 * @file hmplatf.h
 *
 * Platform differentation macros
 *
 * @if NOTICE
 *
 * $Id: hmplatf.h,v 1.19 2009/03/31 01:44:42 henrik Exp $
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


#ifndef _HMPLATF_H_INCLUDED
#define _HMPLATF_H_INCLUDED


/**
 * @defgroup hm_platf Platform and Compiler Specific Coding Support
 *
 * It is good practice to avoid compiler or platform specific
 * implementations as much as reasonable. However if some program sections
 * have to be implemented differently for particular compilers, then the
 * following macros definitions are available to embed compiler specific
 * code in #ifdef/#endif preprocessor blocks.
 *
 * @remark Platforms currently supported:
 * - Windows (Desktop, Server, CE, Mobile)
 * - MAC OS X
 * - Linux
 * - uClinux
 * - Cygwin
 * - SGI IRIX using MIPSpro compiler
 * - DEC True 64/Digital UNIX using DEC CXX
 * - SCO Openserver using skunkware gcc compiler or UDK C++ Compiler
 * - SUN Solaris/SunOS using SunPro compiler
 * - IBM AIX using xlC compiler
 * - HP-UX using aCC compiler
 * - QNX Neutrino 6
 * - QNX 4
 * - VxWorks
 * - MS-DOS
 * - Keil C51 
 * - AVR using gcc
 *
 * <h2>Compiler Specific Coding</h2>
 *
 * Definitions which can be used to implement code compiler specific:
 *
 * <table>
 *   <tr><td><b>Macro</b></td><td><b>Compiler</b></td></tr>
 *   <tr><td>_MSC_VER</td><td>MS C/Visual C++</td></tr>
 *   <tr><td>__TURBOC__</td><td>Turbo C/C++</td></tr>
 *   <tr><td>__BORLANDC__</td><td>Borland C++</td></tr>
 *   <tr><td> __GNUC__</td><td>GNU GCC/EGCS</td></tr>
 *   <tr><td>__WATCOMC__</td><td>Watcom C/C++</td></tr>
 *   <tr><td>__MWERKS__</td><td>Metrowerks C/C++</td></tr>
 *   <tr><td>__SUNPRO_CC</td><td>SUN Pro C++</td></tr>
 *   <tr><td>__DECC</td><td>DEC CXX</td></tr>
 *   <tr><td>__xlC__</td><td>IBM xlC</td></tr>
 *   <tr><td>__IBMC__</td><td>IBM Visual Age</td></tr>
 *   <tr><td>__HP_aCC</td><td>HP ANSI C++ Compiler</td></tr>
 *   <tr><td>__USLC__</td><td>SCO UDK C++ Compiler</td></tr>
 *   <tr><td>__KEIL__</td><td>Keil C51</td></tr>
 * </table>
 *
 * <h2>Platform Specific Coding</h2>
 *
 * The following set of platform macros helps writing modules which
 * can be compiled differently on various platforms.
 *
 * <table>
 *   <tr><td><b>Macro</b></td><td><b>Platform</b></td></tr>
 *   <tr><td>__WIN32__</td><td>Windows applications
 *   (Desktop, Server, CE, Mobile)</td>
 *   <tr><td>__CYGWIN__</td><td>Cygwin applications </td>
 *   <tr><td>__MACOSX__</td><td>Mac OS X applications</td></tr>
 *   <tr><td>__LINUX__</td><td>Linux applications</td></tr>
 *   <tr><td>__UNIX__</td><td>Standard UNIX/POSIX.1 apps</td></tr>
 *   <tr><td>__NETBSD__</td><td>NetBSD applications</td></tr>
 *   <tr><td>__FREEBSD__</td><td>FreeBSD applications</td></tr>
 *   <tr><td>__OPENBSD__</td><td>OpenBSD applications</td></tr>
 *   <tr><td>__SOLARIS__</td><td>SUN Solaris applications</td></tr>
 *   <tr><td>__IRIX__</td><td>SGI IRIX applications</td></tr>
 *   <tr><td>__OSF__</td><td>DEC Alpha OSF (Tru64 UNIX) applications</td></tr>
 *   <tr><td>__SCO__</td><td>SCO OpenServer applications</td></tr>
 *   <tr><td>__QNX__</td><td>QNX 4 and QNX 6 applications</td></tr>
 *   <tr><td>__QNX4__</td><td>QNX 4 applications</td></tr>
 *   <tr><td>__QNXNTO__</td><td>QNX 6 applications</td></tr>
 *   <tr><td>__VXWORKS__</td><td>VxWorks applications</td></tr>
 *   <tr><td>__MSDOS__</td><td>DOS 16-bit console applications</td></tr>
 *   <tr><td>__C51__</td><td>8051 embedded applications</td></tr>
 *   <tr><td>__AVR__</td><td>AVR embedded applications</td></tr>
 * </table>
 *
 * <h2>Target Types</h2>
 *
 * The following macros should be used to compile different types of
 * targets. They have to be defined manually in the
 * makefile or in the IDE compiler settings.
 *
 * <table>
 *   <tr><td><b>Macro</b></td><td><b>Target Type</b></td></tr>
 *   <tr><td>__GUI__</td><td>Running on a graphical user interface,
 *      no character I/O available</td></tr>
 *   <tr><td>__CONSOLE__</td><td>Standard character I/O
 *      user interface</td></tr>
 *   <tr><td>__EMBEDDED__</td><td>Running without operating system</td></tr>
 *   <tr><td>__DLL__ or _WINDLL</td><td>Compiling as DLL</td></tr>
 * </table>
 *
 */


#undef _HMPLATF_DETECTED


/*
 * Windows applications
 */
#if (defined(_WIN32_WCE) || defined(_WIN32)) && !defined(__CYGWIN__)
#  if !defined(__WIN32__)
#     define __WIN32__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * DOS legacy 16-bit console applications
 * MS C defines _MSDOS, Borland __MSDOS__, Watcom C MSDOS
 */
#if defined(MSDOS) || defined(_MSDOS) || defined(__MSDOS__)
#  if !defined(__MSDOS__)
#     define __MSDOS__
#  endif
#  define __CONSOLE__
#  define _HMPLATF_DETECTED
#endif


/*
 * Cygwin applications
 */
#if defined(__CYGWIN__)
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * MAC OS X / Darwin
 *
 * __APPLE__,  __MACH__ and __POWERPC__ are predefined by GCC
 */
#if defined(__APPLE__) && defined(__MACH__) && defined(__POWERPC__)
#  if !defined(__MACOSX__)
#     define __MACOSX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * Linux applications
 */
#if defined(linux) || defined(__linux__)
#  if !defined(__LINUX__)
#     define __LINUX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * Free BSD
 */
#if defined(__FreeBSD__)
#  if !defined(__FREEBSD__)
#     define __FREEBSD__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * Open BSD
 */
#if defined(__OpenBSD__)
#  if !defined(__OPENBSD__)
#     define __OPENBSD__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif



/*
 * Net BSD
 */
#if defined(__NetBSD__)
#  if !defined(__NETBSD__)
#     define __NETBSD__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif



/*
 * SGI IRIX applications
 *
 * __sgi and __unix is defined by MIPSPro C/C++
 */
#if defined(sgi) || defined(__sgi)
#  if !defined(__IRIX__)
#     define __IRIX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * Solaris applications
 */
#if defined(sun) || defined(__sun__)
#  if !defined(__SOLARIS__)
#     define __SOLARIS__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * SCO applications
 */
#if defined(__SCO_VERSION__)
#  if !defined(__SCO__)
#     define __SCO__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * DEC Alpha OSF (Tru64 UNIX) applications
 */
#if defined(__osf__) || defined(osf)
#  if !defined(__OSF__)
#     define __OSF__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * HP-UX
 */
#if defined(hpux) || defined(__hpux__)
#  if !defined(__HPUX__)
#     define __HPUX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * IBM AIX Unix
 */
#if defined(_AIX)
#  if !defined(__AIX__)
#     define __AIX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * QNX4 applications
 *
 * __QNX__ is defined by Watcom C
 */
#if defined(__QNX__) && defined(__WATCOMC__) && !defined(__QNXNTO__)
#  if !defined(__QNX4__)
#     define __QNX4__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * QNX 6 applications
 *
 * __QNX__, __QNXNTO__ are defined by gcc for QNX 6
 */
#if defined(__QNX__) && defined(__QNXNTO__)
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * VxWorks
 */
#if defined(__vxworks__) || defined(vxworks)
#  if !defined(__VXWORKS__)
#     define __VXWORKS__
#  endif
#  define _HMPLATF_DETECTED
#endif


/*
 * Keil C51 embedded applications (8-bit)
 */
#if defined(__C51__)
#  define __EMBEDDED__
#  define _HMPLATF_DETECTED
#endif


/*
 * AVR embedded applications (8-bit)
 */
#if defined(__AVR__)
#  define __EMBEDDED__
#  define _HMPLATF_DETECTED
#endif


/*
 * Last resort, try to detect any other UNIX
 */
#if defined(__unix__) || defined (unix) || defined(__unix) || defined(_unix)
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#  define _HMPLATF_DETECTED
#endif


#endif /* ifdef ..._H_INCLUDED */
