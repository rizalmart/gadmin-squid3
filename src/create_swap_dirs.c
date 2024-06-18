/* GADMIN-SQUID - An easy to use GTK+ frontend for the Squid proxy server.
 * Copyright (C) 2006 - 2012 Magnus Loef <magnus-swe@telia.com> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
*/



#include "../config.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets.h"
#include "gettext.h"
#include "functions.h"
#include "commands.h"
#include "reread_conf.h"
#include "show_info.h"
#include "allocate.h"
#include "create_swap_dirs.h"



/* Creates swap directories for the server user/group */
void create_swap_dirs(struct w *widgets)
{
    gchar *cmd, *info;
    gchar *toplevel_swapdir, *first_swapdir;

    /* Create Squids cache/swap directories if they arent already created */
    toplevel_swapdir = g_strdup_printf("%s", TOPLEVEL_SWAP_PATH); /* IE: /var/spool/squid */
    first_swapdir = g_strdup_printf("%s/00", TOPLEVEL_SWAP_PATH);

    /* First make sure we have the toplevel swapdir */
    if( ! file_exists(toplevel_swapdir) )
    {
        cmd = g_strdup_printf("mkdir -p %s", toplevel_swapdir);
        if( ! run_command(cmd) )
        {
            /* This fails if the server is running but, it shouldnt be able to start in the first place */
            printf("Error creating toplevel swap directory here: %s\n", toplevel_swapdir);
        }
        g_free(cmd);
        /* Also chown toplevel_swapdir to user:group */
        cmd = g_strdup_printf("chown %s:%s %s", SERVER_USER, SERVER_GROUP, toplevel_swapdir);
        if( ! run_command(cmd) )
        {
            printf("Error changing owner and group to squid on the toplevel swap directory: %s\n", toplevel_swapdir);
        }
        g_free(cmd);
    }
    
    if( ! file_exists(first_swapdir) )
    {
        cmd = g_strdup_printf("%s -z", SQUID_BINARY);
        if( ! run_command(cmd) )
        {
            printf("Error creating cache/swap directories here: %s\n", toplevel_swapdir);
        }
        else
        {
            printf("Created initial cache/swap directories here: %s\n", toplevel_swapdir);

	    info = g_strdup_printf(_("Created initial cache/swap directories here: %s\n"), toplevel_swapdir);
	    show_info(info);
	    g_free(info);
        }
        g_free(cmd);
    }

    g_free(toplevel_swapdir);
    g_free(first_swapdir);

    return;
}
