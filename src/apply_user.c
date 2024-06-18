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




#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets.h"
#include "gettext.h"
#include "functions.h"
#include "reread_conf.h"
#include "show_info.h"
#include "apply_user.h"
#include "allocate.h"
#include "populate_users.h"
#include "populate_user_settings.h"
#include "populate_conf_tab.h"


extern char global_server_address[1024];
extern char global_server_port[1024];
extern char global_server_type[1024];

extern int activated;



/* Fixme mod: group, comment and shell */
void apply_user(struct w *widgets)
{
    gchar *info;

    G_CONST_RETURN gchar *username;
    G_CONST_RETURN gchar *groupname;
    G_CONST_RETURN gchar *password;
    G_CONST_RETURN gchar *comment;

    info = g_strdup_printf("This function is incomplete, nothing was changed\n");
    show_info(info);
    g_free(info);
    return;

    /* Get the settings from the entries */
    username = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[0]));
    password = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[1]));
    groupname = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[2]));
    comment = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[3]));

//    if( password!=NULL )
//      password_length = strlen(password);

    /* If the user has written a password thats less then MIN_PASS_LEN chars long */
//    if( password_length > 0 && password_length < MIN_PASS_LEN )
//    {
//        info = g_strdup_printf(_("The minimum password length is %d.\n"), MIN_PASS_LEN);
//	show_info(info);
//	g_free(info);
//        return;
//    }


    /* Update the userlist */
    populate_users(widgets);

    /* Populate the user settings (it also populates the directories) */
    populate_user_settings(widgets);

//    if( activated )
//      reread_conf(widgets);
}
