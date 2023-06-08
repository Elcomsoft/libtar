/*
**  Copyright 2002-2003 University of Illinois Board of Trustees
**  Copyright 2002-2003 Mark D. Roth
**  All rights reserved.
**
**  internal.h - internal header file for libtar
**
**  Mark D. Roth <roth@uiuc.edu>
**  Campus Information Technologies and Educational Services
**  University of Illinois at Urbana-Champaign
*/

#ifndef INTERNAL_H
#define INTERNAL_H

#include <config.h>
#include <compat.h>

#include <libtar.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _MSC_VER                 /* MS <sys/stat.h> uses this trigger */
typedef unsigned short mode_t;  /* POSIX defines mode_t in <sys/types.h> */
#endif /* _MSCVER */

#ifndef S_IRWXU
/* POSIX-compliant defines omitted from sys/stat.h. */
/* Assume UNIX Version 7 header, more or less       */

#ifndef S_IREAD
#define S_IREAD     0400
#endif /* S_IREAD */
#ifndef S_IWRITE
#define S_IWRITE    0200
#endif /* S_IWRITE */
#ifndef S_IEXEC
#define S_IEXEC     0100
#endif /* S_IEXEC */

#define S_IRUSR (S_IREAD)
#define S_IWUSR (S_IWRITE)
#define S_IXUSR (S_IEXEC)
#define S_IRGRP (S_IRUSR >> 3)
#define S_IWGRP (S_IWUSR >> 3)
#define S_IXGRP (S_IXUSR >> 3)
#define S_IROTH (S_IRUSR >> 6)
#define S_IWOTH (S_IWUSR >> 6)
#define S_IXOTH (S_IXUSR >> 6)
#define S_IRWXU (S_IRUSR|S_IWUSR|S_IXUSR)
#define S_IRWXG (S_IRGRP|S_IWGRP|S_IXGRP)
#define S_IRWXO (S_IROTH|S_IWOTH|S_IXOTH)
#endif /* S_IRWXU */

#ifndef S_ISUID
#  define S_ISUID 04000
#endif
#ifndef S_ISGID
#  define S_ISGID 02000
#endif
#ifndef S_ISVTX
#  define S_ISVTX 01000
#endif

#ifndef S_IFLNK
# define S_IFLNK  0120000
#endif

#ifndef S_IFSOCK
# define S_IFSOCK 0140000
#endif


/*
** Assume the S_IFMT, S_IFDIR, S_IFCHR, S_IFREG are defined, which is OK
** for Windows under MS Visual Studio 2008 Express.  Conditional testing
** not perfect: it assumes pragmatically that S_ISFIFO() is undefined if
** S_IFIFO() is undefined, for example.
*/

/* POSIX file type test macros */
#ifndef S_ISDIR
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#endif /* S_ISDIR */
#ifndef S_ISCHR
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#endif /* S_ISCHR */
#ifndef S_ISREG
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#endif /* S_ISREG */

/* Not in MS Visual Studio 2008 Express */
#ifdef S_IFBLK
#ifndef S_ISBLK
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#endif /* S_ISBLK */
#else
#define S_ISBLK(m)  (0)
#endif /* S_IFBLK */

/* Not in Version 7 Unix, but we'll define it anyway */
#ifdef S_IFIFO
#ifndef S_ISFIFO
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#endif /* S_ISFIFO */
#else
#define S_ISFIFO(m) (0)
#endif /* S_IFIFO */

/* Not in POSIX 1003.1-1990, but we'll define it anyway */
#ifdef S_IFLNK
#ifndef S_ISLNK
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#endif /* S_ISLNK */
#else
#define S_ISLNK(m)  (0)
#endif /* S_IFLNK */

/* Not in POSIX 1003.1-1990, but we'll define it anyway */
#ifdef S_IFSOCK
#ifndef S_ISSOCK
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#endif /* S_ISSOCK */
#else
#define S_ISSOCK(m) (0)
#endif /* S_IFSOCK */


#ifdef TLS
#define TLS_THREAD TLS
#else
#define TLS_THREAD
#endif

char* safer_name_suffix(char const*);

#endif
