/*
 *  Argyle WebSockets
 *  Copyright (C) 2023 David M. Syzdek <david@syzdek.net>.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of David M. Syzdek nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL DAVID M SYZDEK BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */
/*
 *  @file lib/libargylews/lmisc.c
 */
#define _LIB_LIBARGYLEWS_LVERSION_C 1
#include "lversion.h"

///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Headers
#endif

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Definitions
#endif


#ifndef GIT_PACKAGE_MAJOR
#   define GIT_PACKAGE_MAJOR 0
#endif
#ifndef GIT_PACKAGE_MINOR
#   define GIT_PACKAGE_MINOR 0
#endif
#ifndef GIT_PACKAGE_PATCH
#   define GIT_PACKAGE_PATCH 0
#endif
#ifndef GIT_PACKAGE_BUILD
#   define GIT_PACKAGE_BUILD "unknown"
#endif
#ifndef GIT_PACKAGE_VERSION
#   define GIT_PACKAGE_VERSION "0.0.0"
#endif
#ifndef GIT_PACKAGE_VERSION_BUILD
#   define GIT_PACKAGE_VERSION_BUILD "0.0.0.unknown"
#endif

#ifndef LIB_VERSION_CURRENT
#   define LIB_VERSION_CURRENT 0
#endif
#ifndef LIB_VERSION_AGE
#   define LIB_VERSION_AGE 0
#endif
#ifndef LIB_VERSION_REVISION
#   define LIB_VERSION_REVISION 0
#endif
#ifndef LIB_VERSION_INFO
#   define LIB_VERSION_INFO "0:0:0"
#endif


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Variables
#endif

const char *         argylews_build                   = GIT_PACKAGE_BUILD;
unsigned             argylews_major                   = GIT_PACKAGE_MAJOR;
unsigned             argylews_minor                   = GIT_PACKAGE_MINOR;
unsigned             argylews_patch                   = GIT_PACKAGE_PATCH;
const char *         argylews_version                 = GIT_PACKAGE_VERSION;
const char *         argylews_version_build           = GIT_PACKAGE_VERSION_BUILD;
const char *         argylews_version_info            = LIB_VERSION_INFO;
unsigned             argylews_version_info_age        = LIB_VERSION_AGE;
unsigned             argylews_version_info_current    = LIB_VERSION_CURRENT;
unsigned             argylews_version_info_revision   = LIB_VERSION_REVISION;


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Functions
#endif

int
argylews_version_info_check(
         unsigned                      current,
         unsigned                      age )
{
   assert(current >= age);
   if (LIB_VERSION_CURRENT > current)
      return( ((LIB_VERSION_CURRENT-LIB_VERSION_AGE) >= current) ? 0 : 1 );
   return( ((current-age) >= LIB_VERSION_CURRENT) ? 0 : 1 );
}


/* end of source file */
