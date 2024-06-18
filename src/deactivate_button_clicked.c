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
#include <string.h>
#include "widgets.h"
#include "gettext.h"
#include "commands.h"
#include "show_info.h"
#include <stdlib.h>
#include "allocate.h"

extern int activated;


void deactivate_button_clicked(struct w *widgets)
{
    gchar *cmd, *info;

    init_stop(widgets);

    if( ! activated )
      return;

    /* Signal "-15" wont make squid exit using FC11 SQUID Version 3.0.STABLE18 */
//    cmd = g_strdup_printf("killall -15 %s", SQUID_BINARY);
//    cmd = g_strdup_printf("killall -9 %s", SQUID_BINARY);
    cmd = g_strdup_printf("%s -k kill", SQUID_BINARY);
    if( ! run_command(cmd) )
    {
	info = g_strdup_printf("Failed to deactivate the server with command:\n%s\n", cmd);
	show_info(info);
	g_free(info);
    }
    g_free(cmd);
}
