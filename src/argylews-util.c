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
 *  @file src/argylews-util.c
 */
#define _SRC_ARGYLEWS_UTIL_C 1

///////////////
//           //
//  Headers  //
//           //
///////////////
#pragma mark - Headers

#include <argylews_compat.h>

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

#include "argylews-util.h"


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#pragma mark - Definitions

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "argylews"
#endif
#ifndef PACKAGE_BUGREPORT
#define PACKAGE_BUGREPORT "david@syzdek.net"
#endif
#ifndef PACKAGE_COPYRIGHT
#define PACKAGE_COPYRIGHT ""
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME ""
#endif
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION ""
#endif

#undef ARGYLEWS_SHORT_OPT
#define ARGYLEWS_SHORT_OPT "hqVv"


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#pragma mark - Prototypes

int
main(
         int                           argc,
         char *                        argv[] );


//--------------------------//
// miscellaneous prototypes //
//--------------------------//
#pragma mark miscellaneous prototypes

static argws_widget_t *
argws_widget_lookup(
         const char *                  wname,
         int                           exact );


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#pragma mark - Variables

#pragma mark argws_widget_map[]
static argws_widget_t argws_widget_map[] =
{
#ifdef USE_DEBUG
   // debug widget
   {  .name       = "debug",
      .desc       = NULL,
      .usage      = NULL,
      .short_opt  = "0123456789abc:d:efghijk:lm:nopqrs:t:uvwx:yzABCDEFGHIJKLMNO:P:QRST:UVWXYZ",
      .arg_min    = 0,
      .arg_max    = 4096,
      .aliases    = NULL,
      .func_exec  = &argws_widget_debug,
      .func_usage = NULL,
   },
#endif

   // help widget
   {  .name       = "help",
      .desc       = "display help",
      .usage      = NULL,
      .short_opt  = NULL,
      .aliases    = NULL,
      .func_exec  = &argws_widget_usage,
      .func_usage = NULL,
   },

   // version widget
   {  .name       = "version",
      .desc       = "display version",
      .usage      = NULL,
      .short_opt  = NULL,
      .aliases    = NULL,
      .func_exec  = &argws_widget_version,
      .func_usage = NULL,
   },

   {  .name       = NULL,
      .desc       = NULL,
      .usage      = NULL,
      .short_opt  = NULL,
      .aliases    = NULL,
      .func_exec  = NULL,
      .func_usage = NULL,
   },
};


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#pragma mark - Functions


//---------------//
// main function //
//---------------//
#pragma mark main function

int
main(
         int                           argc,
         char *                        argv[] )
{
   int                  rc;
   argws_config_t *     cnf;
   argws_config_t       config;


   // initialize config
   cnf = &config;
   memset(cnf, 0, sizeof(argws_config_t));


   // determine program name
   if ((cnf->prog_name = strrchr(argv[0], '/')) != NULL)
      cnf->prog_name = &cnf->prog_name[1];
   if (!(cnf->prog_name))
      cnf->prog_name = argv[0];


   // skip argument processing if called via alias
   if ((cnf->widget = argws_widget_lookup(cnf->prog_name, 1)) != NULL)
   {
      cnf->argc      = argc;
      cnf->argv      = argv;
      cnf->symlinked = 1;
      return(cnf->widget->func_exec(cnf));
   };


   // initial processing of cli arguments
   if ((rc = argws_arguments(cnf, argc, argv)) != 0)
      return((rc == -1) ? 0 : 1);
   if ((argc - optind) < 1)
   {
      fprintf(stderr, "%s: missing required argument\n", cnf->prog_name);
      fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
      return(1);
   };
   cnf->argc   = (argc - optind);
   cnf->argv   = &argv[optind];


   // looks up widget
   if ((cnf->widget = argws_widget_lookup(argv[optind], 0)) == NULL)
   {
      fprintf(stderr, "%s: unknown or ambiguous widget -- \"%s\"\n", cnf->prog_name, cnf->argv[0]);
      fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
      return(1);
   };


   return(cnf->widget->func_exec(cnf));
}


//-------------------------//
// miscellaneous functions //
//-------------------------//
#pragma mark miscellaneous functions

int
argws_arguments(
         argws_config_t *              cnf,
         int                           argc,
         char * const *                argv )
{
   int            c;
   int            opt_index;
   uint64_t       uval;
   char *         endptr;
   int            i;

   // getopt options
   static const char *  short_opt = "+" ARGYLEWS_SHORT_OPT;
   static struct option long_opt[] =
   {
      {"help",             no_argument,       NULL, 'h' },
      {"quiet",            no_argument,       NULL, 'q' },
      {"silent",           no_argument,       NULL, 'q' },
      {"version",          no_argument,       NULL, 'V' },
      {"verbose",          no_argument,       NULL, 'v' },
      { NULL, 0, NULL, 0 }
   };

   optind    = 1;
   opt_index = 0;

   if ((cnf->widget))
      short_opt = ((cnf->widget->short_opt)) ? cnf->widget->short_opt : ARGYLEWS_SHORT_OPT;

   while((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -1:       /* no more arguments */
         case 0:        /* long options toggles */
         break;

         case 'c':
         uval = strtoull(optarg, &endptr, 0);
         i    = (int)uval;
         if ((optarg == endptr) || (endptr[0] != '\0'))
         {
            fprintf(stderr, "%s: invalid value for `-c'\n", PROGRAM_NAME);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
         break;

         case 'h':
         argws_widget_usage(cnf);
         return(-1);

         case 'q':
         cnf->quiet = 1;
         if ((cnf->verbose))
         {
            fprintf(stderr, "%s: incompatible options `-q' and `-v'\n", PROGRAM_NAME);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
         break;

         case 'V':
         argws_widget_version(cnf);
         return(-1);

         case 'v':
         cnf->verbose++;
         if ((cnf->quiet))
         {
            fprintf(stderr, "%s: incompatible options `-q' and `-v'\n", PROGRAM_NAME);
            fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
            return(1);
         };
         break;

         case '?':
         fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n", PROGRAM_NAME, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
         return(1);
      };
   };

   if (!(cnf->widget))
      return(0);

   if ( (cnf->argc-optind) < cnf->widget->arg_min)
   {
      fprintf(stderr, "%s: missing required argument\n", PROGRAM_NAME);
      fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
      return(1);
   };
   if ( (cnf->argc-optind) > cnf->widget->arg_max)
   {
      fprintf(stderr, "%s: unknown argument -- `%s'\n", PROGRAM_NAME, cnf->argv[optind + cnf->widget->arg_max]);
      fprintf(stderr, "Try `%s --help' for more information.\n", PROGRAM_NAME);
      return(1);
   };

   return(0);
}


argws_widget_t *
argws_widget_lookup(
         const char *                  wname,
         int                           exact )
{
   int                        x;
   int                        y;
   size_t                     len;
   size_t                     wname_len;
   const char *               alias;
   argws_widget_t *           match;
   argws_widget_t *           widget;

   // strip program prefix from widget name
   len = strlen(PROGRAM_NAME);
   if (!(strncasecmp(wname, PROGRAM_NAME, len)))
      wname = &wname[len];
   if (wname[0] == '-')
      wname = &wname[1];
   if (!(wname[0]))
      return(NULL);

   match       = NULL;
   wname_len   = strlen(wname);

   for(x = 0; ((argws_widget_map[x].name)); x++)
   {
      // check widget
      widget = &argws_widget_map[x];
      if (widget->func_exec == NULL)
         continue;
      widget->alias_idx = -1;

      // compare widget name for match
      if (!(strncmp(widget->name, wname, wname_len)))
      {
         if (widget->name[wname_len] == '\0')
            return(widget);
         if ( ((match)) && (match != widget) )
            return(NULL);
         if (exact == 0)
            match = widget;
      };

      if (!(widget->aliases))
         continue;

      for(y = 0; ((widget->aliases[y])); y++)
      {
         alias = widget->aliases[y];
         if (!(strncmp(alias, wname, wname_len)))
         {
            if (alias[wname_len] == '\0')
            {
               widget->alias_idx = y;
               return(widget);
            };
            if ( ((match)) && (match != widget) )
               return(NULL);
            if (exact == 0)
               match = widget;
         };
      };
   };

   return((exact == 0) ? match : NULL);
}


//-------------------//
// widgets functions //
//-------------------//
#pragma mark widgets functions

int
argws_widget_debug(
         argws_config_t *            cnf )
{
   int      rc;
   int      pos;

   // initial processing of cli arguments
   if ((rc = argws_arguments(cnf, cnf->argc, cnf->argv)) != 0)
      return((rc == -1) ? 0 : 1);
   if (optind != cnf->argc)
   {
      printf("Arguments:\n");
      for(pos = optind; (pos < cnf->argc); pos++)
         printf("   %2i: \"%s\"\n", pos, cnf->argv[pos]);
   };

   printf("\n");

   return(0);
}


/// displays usage information
int
argws_widget_usage(
         argws_config_t *            cnf )
{
   size_t                     pos;
   const char *               widget_name;
   const char *               widget_help;
   const char *               short_opt;
   const argws_widget_t *     widget;

   assert(cnf != NULL);

   widget_name  = (!(cnf->widget)) ? "widget"               : cnf->widget->name;
   short_opt    = ((cnf->widget))  ? cnf->widget->short_opt : ARGYLEWS_SHORT_OPT;
   short_opt    = ((short_opt))    ? short_opt              : ARGYLEWS_SHORT_OPT;
   widget_help  = "";
   if ((cnf->widget))
      widget_help = ((cnf->widget->usage)) ? cnf->widget->usage : "";

   if ((widget = cnf->widget) == NULL)
   {
      printf("Usage: %s %s [OPTIONS]%s\n", PROGRAM_NAME, widget_name, widget_help);
      printf("       %s-%s [OPTIONS]%s\n", PROGRAM_NAME, widget_name, widget_help);
      printf("       %s%s [OPTIONS]%s\n",  PROGRAM_NAME, widget_name, widget_help);
   } else if (cnf->symlinked == 0)
   {
      widget_name = (widget->alias_idx == -1) ? widget_name : widget->aliases[widget->alias_idx];
      printf("Usage: %s %s [OPTIONS]%s\n", PROGRAM_NAME, widget_name, widget_help);
   }
   else
      printf("Usage: %s [OPTIONS]%s\n", cnf->prog_name, widget_help);
   printf("OPTIONS:\n");
   if ((strchr(short_opt, '1'))) printf("  -1                        use HTTP/1.1\n");
   if ((strchr(short_opt, '2'))) printf("  -2                        use HTTP/2\n");
   if ((strchr(short_opt, 'H'))) printf("  -H uri                    URI of websocket server\n");
   if ((strchr(short_opt, 'h'))) printf("  -h, --help                print this help and exit\n");
   if ((strchr(short_opt, 'q'))) printf("  -q, --quiet, --silent     do not print messages\n");
   if ((strchr(short_opt, 'V'))) printf("  -V, --version             print version number and exit\n");
   if ((strchr(short_opt, 'v'))) printf("  -v, --verbose             print verbose messages\n");

   if (!(cnf->widget))
   {
      printf("WIDGETS:\n");
      for(pos = 0; argws_widget_map[pos].name != NULL; pos++)
      {
         widget = &argws_widget_map[pos];
         if ((widget->desc))
            printf("  %-25s %s\n", widget->name, widget->desc);
      };
   };

   if ((cnf->widget))
      if ((cnf->widget->func_usage))
         cnf->widget->func_usage(cnf);

   printf("\n");

   return(0);
}


/// displays version information
int
argws_widget_version(
         argws_config_t *            cnf )
{
   assert(cnf != NULL);
   printf(
      (
         "%s (%s) %s\n"
         "Written by David M. Syzdek.\n"
      ), PROGRAM_NAME, PACKAGE_NAME, PACKAGE_VERSION
   );
   return(0);
}


/* end of source file */
