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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <stdio.h>
#include <string.h>
#include "vi.h"
#include "win.h"
#include "keys.h"


/*
 * DoGenericFilter - filter some crap
 */
int DoGenericFilter( linenum s, linenum e, char *cmd )
{
    fcb         *cfcb,*s1fcb,*e1fcb,*tfcb;
    line        *cline;
    FILE        *f;
    int         rc;
    char        realcmd[MAX_STR];
    char        filtin[L_tmpnam], filtout[L_tmpnam];

    rc = ModificationTest();
    if( rc != ERR_NO_ERR ) {
        return( rc );
    }

    /*
     * get file
     */
    tmpnam( filtin );
    tmpnam( filtout );
    f = fopen( filtin, "w" );
    if( f==NULL ) {
        return( ERR_FILE_OPEN );
    }

    /*
     * filter on a line
     */
    rc = GetCopyOfLineRange( s,e, &s1fcb, &e1fcb );
    if( rc ) {
        fclose( f );
        return( rc );
    }

    /*
     * now, dump this crap to a tmp file
     */
    cfcb = s1fcb;
    while( cfcb != NULL ) {
        FetchFcb( cfcb );
        cline = cfcb->line_head;
        while( cline != NULL ) {
            MyFprintf( f, "%s\n", cline->data );
            cline = cline->next;
        }
        tfcb = cfcb;
        cfcb = cfcb->next;
        FcbFree( tfcb );
    }
    fclose( f );

    /*
     * shell out to the given command
     */
    rc = ExecCmd( filtin, filtout, cmd );
    StartUndoGroup( UndoStack );
    rc = DeleteLineRange( s, e, 0 );
    if( !rc ) {
        strcpy( realcmd, filtout );
        ReadAFile( s-1, realcmd );
        Message1( "%l lines filtered through %s",e-s+1, cmd );
    }
    EndUndoGroup( UndoStack );

    /*
     * cleanup
     */
    if( rc == ERR_NO_ERR ) {
        rc = DO_NOT_CLEAR_MESSAGE_WINDOW;
    }
    remove( filtin );
    remove( filtout );
    return( rc );

} /* DoGenericFilter */
