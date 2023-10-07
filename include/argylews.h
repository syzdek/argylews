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
 *  @file include/argylews.h
 */
#ifndef __ARGYLEWS_H
#define __ARGYLEWS_H 1
#undef ARGYLEWS_PMARK


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Headers
#endif

#include <stddef.h>
#include <inttypes.h>
#include <sys/types.h>
#include <stdio.h>
#include <regex.h>


//////////////
//          //
//  Macros  //
//          //
//////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Macros
#endif

// Exports function type
#undef ARGYLEWS_C_DECLS
#undef ARGYLEWS_BEGIN_C_DECLS
#undef ARGYLEWS_END_C_DECLS
#undef _ARGYLEWS_I
#undef _ARGYLEWS_F
#undef _ARGYLEWS_V
#if defined(__cplusplus) || defined(c_plusplus)
#   define _ARGYLEWS_I             extern "C" inline
#   define ARGYLEWS_C_DECLS        "C"             ///< exports as C functions
#   define ARGYLEWS_BEGIN_C_DECLS  extern "C" {    ///< exports as C functions
#   define ARGYLEWS_END_C_DECLS    }               ///< exports as C functions
#else
#   define _ARGYLEWS_I             inline
#   define ARGYLEWS_C_DECLS        /* empty */     ///< exports as C functions
#   define ARGYLEWS_BEGIN_C_DECLS  /* empty */     ///< exports as C functions
#   define ARGYLEWS_END_C_DECLS    /* empty */     ///< exports as C functions
#endif
#ifdef WIN32
#   ifdef _LIB_LIBARGYLEWS_H
#      define _ARGYLEWS_F   extern ARGYLEWS_C_DECLS __declspec(dllexport)  ///< used for library calls
#      define _ARGYLEWS_V   extern ARGYLEWS_C_DECLS __declspec(dllexport)  ///< used for library calls
#   else
#      define _ARGYLEWS_F   extern ARGYLEWS_C_DECLS __declspec(dllimport)  ///< used for library calls
#      define _ARGYLEWS_V   extern ARGYLEWS_C_DECLS __declspec(dllimport)  ///< used for library calls
#   endif
#else
#   ifdef _LIB_LIBARGYLEWS_H
#      define _ARGYLEWS_F   /* empty */               ///< used for library calls
#      define _ARGYLEWS_V   extern ARGYLEWS_C_DECLS   ///< used for library calls
#   else
#      define _ARGYLEWS_F   extern ARGYLEWS_C_DECLS   ///< used for library calls
#      define _ARGYLEWS_V   extern ARGYLEWS_C_DECLS   ///< used for library calls
#   endif
#endif


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Definitions
#endif

#define ARGYLEWS_SUCCESS            0x0000 ///< operation was successful


/////////////////
//             //
//  Datatypes  //
//             //
/////////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Datatypes
#endif


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Variables
#endif

extern const char *                    argylews_build;
extern unsigned                        argylews_major;
extern unsigned                        argylews_minor;
extern unsigned                        argylews_patch;
extern const char *                    argylews_version;
extern const char *                    argylews_version_build;
extern const char *                    argylews_version_info;
extern unsigned                        argylews_version_info_age;
extern unsigned                        argylews_version_info_current;
extern unsigned                        argylews_version_info_revision;


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef ARGYLEWS_PMARK
#   pragma mark - Prototypes
#endif
ARGYLEWS_BEGIN_C_DECLS

//--------------------//
// version prototypes //
//--------------------//
#ifdef ARGYLEWS_PMARK
#   pragma mark version prototypes
#endif

_ARGYLEWS_F int
argylews_version_info_check(
         unsigned                      current,
         unsigned                      age );


ARGYLEWS_END_C_DECLS
#endif /* end of header file */
