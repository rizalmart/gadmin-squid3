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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "gettext.h"
#include "widgets.h"
#include "commands.h"
#include "add_standard_users.h"
#include "functions.h"
#include "show_info.h"



void add_standard_users()
{
    /* The system SERVER_USER and SERVER_GROUP must exist so add them if they are missing */
    gchar *cmd, *info, *user, *group;

    group = g_strdup_printf("%s", SERVER_GROUP);

    /* Add SERVER_GROUP if it doesnt exist */
    if( ! group_exists(group) )
    {
        cmd = g_strdup_printf("%s %s", "groupadd", group);
        if( ! run_command(cmd) )
        {
    	    info = g_strdup_printf(_("Failed adding the system group:\n%s\n"), group);
    	    show_info(info);
    	    g_free(info);

            g_free(cmd);
            g_free(group);
            return;
        }
        g_free(cmd);
    }

    /* Add SERVER_USER to SERVER_GROUP if it doesnt exist */
    user  = g_strdup_printf("%s", SERVER_USER);
    if( ! user_exists(user) )
    {
        /* Add the SERVER_USER to the SERVER_GROUP and lock the account */
        cmd = g_strdup_printf("%s '%s' -l -d '/dev/null' -c '%s' -s '/dev/null' -g '%s'", "useradd", user, user, group);
        if( ! run_command(cmd) )
        {
    	    info = g_strdup_printf(_("Failed adding the system user:\n%s\n"), user);
    	    show_info(info);
    	    g_free(info);

            g_free(cmd);
            g_free(user);
            g_free(group);
            return;
        }
        g_free(cmd);

	/* To be really sure we check it again with the same function */
	if( ! user_exists(user) )
	{
    	    info = g_strdup_printf(_("Failed adding the system user:\n%s\n"), user);
    	    show_info(info);
    	    g_free(info);

    	    g_free(user);
            g_free(group);
    	    return;
	}
	else
	  {
	      /* Let the user know that we added a locked system user */
    	      info = g_strdup_printf(_("Added this locked and false shelled system user:\n%s\n"), user);
    	      show_info(info);
    	      g_free(info);
	  }
    }

    g_free(user);
    g_free(group);
}
