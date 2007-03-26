/*
 * Name:        wx/chartype.h
 * Purpose:     Declarations of wxChar and related types
 * Author:      Joel Farley, Ove K�ven
 * Modified by: Vadim Zeitlin, Robert Roebling, Ron Lee
 * Created:     1998/06/12
 * RCS-ID:      $Id$
 * Copyright:   (c) 1998-2006 wxWidgets dev team
 * Licence:     wxWindows licence
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#ifndef _WX_WXCHARTYPE_H_
#define _WX_WXCHARTYPE_H_

/* defs.h indirectly includes this file, so don't include it here */
#include "wx/platform.h"

/* check whether we have wchar_t and which size it is if we do */
#if !defined(wxUSE_WCHAR_T)
    #if defined(__UNIX__)
        #if defined(HAVE_WCSTR_H) || defined(HAVE_WCHAR_H) || defined(__FreeBSD__) || defined(__DARWIN__)
            #define wxUSE_WCHAR_T 1
        #else
            #define wxUSE_WCHAR_T 0
        #endif
    #elif defined(__GNUWIN32__) && !defined(__MINGW32__)
        #define wxUSE_WCHAR_T 0
    #elif defined(__WATCOMC__)
        #define wxUSE_WCHAR_T 0
    #elif defined(__VISAGECPP__) && (__IBMCPP__ < 400)
        #define wxUSE_WCHAR_T 0
    #else
        /* add additional compiler checks if this fails */
        #define wxUSE_WCHAR_T 1
    #endif
#endif /* !defined(wxUSE_WCHAR_T) */

/* Unicode support requires wchar_t */
#if wxUSE_UNICODE && !wxUSE_WCHAR_T
    #error "wchar_t must be available in Unicode build"
#endif /* Unicode */

/*
   non Unix compilers which do have wchar.h (but not tchar.h which is included
   below and which includes wchar.h anyhow).

   Actually MinGW has tchar.h, but it does not include wchar.h
 */
#if defined(__MWERKS__) || defined(__VISAGECPP__) || defined(__MINGW32__) || defined(__WATCOMC__)
    #ifndef HAVE_WCHAR_H
        #define HAVE_WCHAR_H
    #endif
#endif
#if defined(__MWERKS__) && !defined(__MACH__)
    #ifndef HAVE_WCSLEN
        #define HAVE_WCSLEN
    #endif
#endif

#if wxUSE_WCHAR_T
    #ifdef HAVE_WCHAR_H
        /* the current (as of Nov 2002) version of cygwin has a bug in its */
        /* wchar.h -- there is no extern "C" around the declarations in it */
        /* and this results in linking errors later; also, at least on some */
        /* Cygwin versions, wchar.h requires sys/types.h */
        #ifdef __CYGWIN__
            #include <sys/types.h>
            #ifdef __cplusplus
                extern "C" {
            #endif
        #endif /* Cygwin */

        #include <wchar.h>

        #if defined(__CYGWIN__) && defined(__cplusplus)
            }
        #endif /* Cygwin and C++ */

    #elif defined(HAVE_WCSTR_H)
        /* old compilers have relevant declarations here */
        #include <wcstr.h>
    #elif defined(__FreeBSD__) || defined(__DARWIN__) || defined(__EMX__)
        /* include stdlib.h for wchar_t */
        #include <stdlib.h>
    #endif /* HAVE_WCHAR_H */

    #ifdef HAVE_WIDEC_H
        #include <widec.h>
    #endif
#endif /* wxUSE_WCHAR_T */

/* -------------------------------------------------------------------------- */
/* define wxHAVE_TCHAR_SUPPORT for the compilers which support the TCHAR type */
/* mapped to either char or wchar_t depending on the ASCII/Unicode mode and   */
/* have the function mapping _tfoo() -> foo() or wfoo()                       */
/* -------------------------------------------------------------------------- */

/* VC++ and BC++ starting with 5.2 have TCHAR support */
#ifdef __VISUALC__
    #define wxHAVE_TCHAR_SUPPORT
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x520)
    #define wxHAVE_TCHAR_SUPPORT
    #include <ctype.h>
#elif defined(__WATCOMC__)
    #define wxHAVE_TCHAR_SUPPORT
#elif defined(__DMC__)
    #define wxHAVE_TCHAR_SUPPORT
#elif defined(__WXPALMOS__)
    #include <stddef.h>
#elif defined(__MINGW32__) && wxCHECK_W32API_VERSION( 1, 0 )
    #define wxHAVE_TCHAR_SUPPORT
    #include <stddef.h>
    #include <string.h>
    #include <ctype.h>
#elif 0 && defined(__VISAGECPP__) && (__IBMCPP__ >= 400)
    /* VZ: the old VisualAge definitions were completely wrong and had no    */
    /*     chance at all to work in Unicode build anyhow so let's pretend    */
    /*     that VisualAge does _not_ support TCHAR for the moment (as        */
    /*     indicated by "0 &&" above) until someone really has time to delve */
    /*     into Unicode issues under OS/2 */

    /* VisualAge 4.0+ supports TCHAR */
    #define wxHAVE_TCHAR_SUPPORT
#endif /* compilers with (good) TCHAR support */

#ifdef wxHAVE_TCHAR_SUPPORT
    /* get TCHAR definition if we've got it */
    #include <tchar.h>

    /* we surely do have wchar_t if we have TCHAR */
    #ifndef wxUSE_WCHAR_T
        #define wxUSE_WCHAR_T 1
    #endif /* !defined(wxUSE_WCHAR_T) */
#endif /* wxHAVE_TCHAR_SUPPORT */

/* ------------------------------------------------------------------------- */
/* define wxChar type                                                        */
/* ------------------------------------------------------------------------- */

/* TODO: define wxCharInt to be equal to either int or wint_t? */

#if !wxUSE_UNICODE
    typedef char wxChar;
    typedef signed char wxSChar;
    typedef unsigned char wxUChar;
#else /* Unicode */
    /* VZ: note that VC++ defines _T[SU]CHAR simply as wchar_t and not as    */
    /*     signed/unsigned version of it which (a) makes sense to me (unlike */
    /*     char wchar_t is always unsigned) and (b) was how the previous     */
    /*     definitions worked so keep it like this                           */

    /* Sun's SunPro compiler supports the wchar_t type and wide character    */
    /* functions, but does not define __WCHAR_TYPE__. Define it here to      */
    /* allow unicode enabled builds.                                         */
    #if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
    #define __WCHAR_TYPE__ wxchar_t
    #endif

    /* GNU libc has __WCHAR_TYPE__ which requires special treatment, see */
    /* comment below */
    #if !defined(__WCHAR_TYPE__) || \
        (!defined(__GNUC__) || wxCHECK_GCC_VERSION(2, 96))
        /* standard case */
        typedef wchar_t wxChar;
        typedef wchar_t wxSChar;
        typedef wchar_t wxUChar;
    #else /* __WCHAR_TYPE__ and gcc < 2.96 */
        /* VS: wxWidgets used to define wxChar as __WCHAR_TYPE__ here.       */
        /*     However, this doesn't work with new GCC 3.x compilers because */
        /*     wchar_t is C++'s builtin type in the new standard. OTOH, old  */
        /*     compilers (GCC 2.x) won't accept new definition of            */
        /*     wx{S,U}CharType, so we have to define wxChar                  */
        /*     conditionally depending on detected compiler & compiler       */
        /*     version.                                                      */

        /*     with old definition of wxChar. */
        #define wchar_t __WCHAR_TYPE__
        typedef __WCHAR_TYPE__ wxChar;
        typedef __WCHAR_TYPE__ wxSChar;
        typedef __WCHAR_TYPE__ wxUChar;
    #endif /* __WCHAR_TYPE__ */
#endif /* ASCII/Unicode */

/* ------------------------------------------------------------------------- */
/* define wxStringCharType                                                   */
/* ------------------------------------------------------------------------- */

/* depending on the platform, Unicode build can either store wxStrings as
   wchar_t* or UTF-8 encoded char*: */
#if wxUSE_UNICODE
    /* for now, all Unicode builds are wchar_t* based: */
    #define wxUSE_UNICODE_WCHAR 1
    #define wxUSE_UNICODE_UTF8  0
#else
    #define wxUSE_UNICODE_WCHAR 0
    #define wxUSE_UNICODE_UTF8  0
#endif

/* define char type used by wxString internal representation: */
#if wxUSE_UNICODE_UTF8
    typedef char wxStringCharType;
#elif wxUSE_UNICODE_WCHAR
    typedef wchar_t wxStringCharType;
#else
    typedef char wxStringCharType;
#endif


/* ------------------------------------------------------------------------- */
/* define _T() and related macros                                            */
/* ------------------------------------------------------------------------- */

/* BSD systems define _T() to be something different in ctype.h, override it */
#if defined(__FreeBSD__) || defined(__DARWIN__)
    #include <ctype.h>
    #undef _T
#endif

/* could already be defined by tchar.h (it's quasi standard) */
#ifndef _T
    #if !wxUSE_UNICODE
        #define _T(x) x
    #else /* Unicode */
        /* use wxCONCAT_HELPER so that x could be expanded if it's a macro */
        #define _T(x) wxCONCAT_HELPER(L, x)
    #endif /* ASCII/Unicode */
#endif /* !defined(_T) */

/* although global macros with such names are normally bad, we want to have  */
/* another name for _T() which should be used to avoid confusion between     */
/* _T() and _() in wxWidgets sources */
#define wxT(x)       _T(x)

/* a helper macro allowing to make another macro Unicode-friendly, see below */
#define wxAPPLY_T(x) _T(x)

/* Unicode-friendly __FILE__, __DATE__ and __TIME__ analogs */
#ifndef __TFILE__
    #define __TFILE__ wxAPPLY_T(__FILE__)
#endif

#ifndef __TDATE__
    #define __TDATE__ wxAPPLY_T(__DATE__)
#endif

#ifndef __TTIME__
    #define __TTIME__ wxAPPLY_T(__TIME__)
#endif

#endif /* _WX_WXCHARTYPE_H_ */

