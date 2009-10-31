/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  OS/2 implementation of stat().
*
****************************************************************************/


#define INCL_LONGLONG
#include "variety.h"
#include "widechar.h"
#include "int64.h"
#undef __INLINE_FUNCTIONS__
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <direct.h>

#include <wos2.h>
#include <dos.h>
#include <mbstring.h>
#include "rtdata.h"
#include "seterrno.h"
#ifdef __WIDECHAR__
    #include <stdlib.h>
    #include <wctype.h>
    #include "mbwcconv.h"
#else
    #include <ctype.h>
#endif
#include "d2ttime.h"

#if defined(__WARP__)
  #define FF_ATTR       ULONG
 #ifdef __INT64__
  #define FF_LEVEL      FIL_STANDARDL
  #define FF_BUFFER     FILEFINDBUF3L
 #else
  #define FF_LEVEL      FIL_STANDARD
  #define FF_BUFFER     FILEFINDBUF3
 #endif
#else
  #define FF_ATTR       USHORT
  #define FF_LEVEL      0
  #define FF_BUFFER     FILEFINDBUF
#endif

static  unsigned short          at2mode(FF_ATTR,char *);


#ifdef __INT64__
 _WCRTLINK int __F_NAME(_stati64,_wstati64)( CHAR_TYPE const *path, struct _stati64 *buf )
#else
 _WCRTLINK int __F_NAME(stat,_wstat)( CHAR_TYPE const *path, struct __F_NAME(stat,_stat) *buf )
#endif
{
    FF_BUFFER           dir_buff;
    CHAR_TYPE const     *ptr;
    HDIR                handle = 1;
    ULONG               drvmap;
    OS_UINT             drive;
    OS_UINT             searchcount = 1;
    APIRET              rc;
    CHAR_TYPE           fullpath[_MAX_PATH];
    int                 isrootdir = 0;

    /* reject null string and names that has wildcard */
    if( *path == NULLCHAR || __F_NAME(_mbspbrk,wcspbrk)( path, STRING( "*?" ) ) != NULL ) {
        __set_errno( ENOENT );
        return( -1 );
    }

    /*** Determine if 'path' refers to a root directory ***/
    if( __F_NAME(_fullpath,_wfullpath)( fullpath, path, _MAX_PATH ) != NULL ) {
        if( __F_NAME(isalpha,iswalpha)( fullpath[0] )  &&  fullpath[1] == STRING( ':' ) &&
            fullpath[2] == STRING( '\\' ) && fullpath[3] == NULLCHAR )
        {
            isrootdir = 1;
        }
    }

    ptr = path;
    if( __F_NAME(*_mbsinc(path),path[1]) == STRING( ':' ) )
        ptr += 2;
    if( ( ptr[0] == STRING( '\\' ) || ptr[0] == STRING( '/' ) ) && ptr[1] == NULLCHAR || isrootdir ) {
        /* handle root directory */
        int             drv;

        /* check if drive letter is valid */
        drv = __F_NAME(tolower,towlower)( *fullpath ) - STRING( 'a' );
        DosQCurDisk( &drive, &drvmap );
        if( ( drvmap & ( 1UL << drv ) ) == 0 ) {
            __set_errno( ENOENT );
            return( -1 );
        }

        dir_buff.attrFile = _A_SUBDIR;           /* fill in DTA */
        *(USHORT *)(&dir_buff.ftimeCreation) = 0;
        *(USHORT *)(&dir_buff.fdateCreation) = 0;
        *(USHORT *)(&dir_buff.ftimeLastAccess) = 0;
        *(USHORT *)(&dir_buff.fdateLastAccess) = 0;
        *(USHORT *)(&dir_buff.ftimeLastWrite) = 0;
        *(USHORT *)(&dir_buff.fdateLastWrite) = 0;
        dir_buff.cbFile = 0;
    } else {    /* not a root directory */
#ifdef __WIDECHAR__
        char        mbPath[MB_CUR_MAX*_MAX_PATH];
        __filename_from_wide( mbPath, path );
#endif
        rc = DosFindFirst( (char*)__F_NAME(path,mbPath), &handle, 0x37,
                           &dir_buff, sizeof( dir_buff ),
                           &searchcount, FF_LEVEL );
        if( rc == ERROR_FILE_NOT_FOUND ) { // appply a bit more persistence
            int handle;

            rc = 0;
            handle = __F_NAME(open,_wopen)( path, O_WRONLY );
            if( handle < 0 ) {
                __set_errno( ENOENT );
                return( -1 );
#ifdef __INT64__
            } else if( __F_NAME(_fstati64,_wfstati64)( handle, buf ) == -1 ) {
#else
            } else if( __F_NAME(fstat,_wfstat)( handle, buf ) == -1 ) {
#endif
                rc = errno;
            }
            close( handle );
            __set_errno( rc );
            if( rc != 0 ) {
                return( -1 );
            }
            return( 0 );
        } else if( rc != 0 || searchcount != 1 ) {
            __set_errno( ENOENT );
            return( -1 );
        }
    }

    /* process drive number */
    if( __F_NAME(*_mbsinc(path),path[1]) == STRING( ':' ) ) {
        buf->st_dev = __F_NAME(tolower,towlower)( *path ) - STRING( 'a' );
    } else {
        DosQCurDisk( &drive, &drvmap );
        buf->st_dev = drive - 1;
    }
    buf->st_rdev = buf->st_dev;

#ifdef __INT64__
    buf->st_size = GET_REALINT64( dir_buff.cbFile );
#else
    buf->st_size = dir_buff.cbFile;
#endif
    buf->st_mode = at2mode( dir_buff.attrFile, dir_buff.achName );

    buf->st_ctime = _d2ttime( TODDATE( dir_buff.fdateCreation ),
                              TODTIME( dir_buff.ftimeCreation ) );
    buf->st_atime = _d2ttime( TODDATE( dir_buff.fdateLastAccess ),
                              TODTIME( dir_buff.ftimeLastAccess ) );
    buf->st_mtime = _d2ttime( TODDATE( dir_buff.fdateLastWrite ),
                              TODTIME( dir_buff.ftimeLastWrite ) );
    buf->st_btime = buf->st_mtime;
    buf->st_nlink = 1;
    buf->st_ino = buf->st_uid = buf->st_gid = 0;

    buf->st_attr = dir_buff.attrFile;
    buf->st_archivedID = 0;
    buf->st_updatedID = 0;
    buf->st_inheritedRightsMask = 0;
    buf->st_originatingNameSpace = 0;

    return( 0 );
}


static unsigned short at2mode( FF_ATTR attr, char *fname ) {

    register unsigned short mode;
    register char * ext;

    if( attr & _A_SUBDIR ) {
        mode = S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;
    } else if( attr & 0x40 ) {
        mode = S_IFCHR;
    } else {
        mode = S_IFREG;
        /* determine if file is executable, very PC specific */
        if( (ext = _mbschr( fname, '.' )) != NULL ) {
            ++ext;
            if( _mbscmp( ext, "EXE" ) == 0 ) {
                mode |= S_IXUSR | S_IXGRP | S_IXOTH;
            }
        }
    }
    mode |= S_IRUSR | S_IRGRP | S_IROTH;
    if( !(attr & _A_RDONLY) )                   /* if file is not read-only */
        mode |= S_IWUSR | S_IWGRP | S_IWOTH;    /* - indicate writeable     */
    return( mode );
}
