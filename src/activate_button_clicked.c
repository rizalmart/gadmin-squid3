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
#include "widgets.h"
#include "commands.h"
#include "activate_button_clicked.h"

extern int activated;



void activate_button_clicked(struct w *widgets)
{
    gchar *start, *test;

    if( activated )
      return;

    /* Start the server */
    start = g_strdup_printf("%s -f %s", SQUID_BINARY, SQUID_CONF);
    if( ! run_command(start) )
    {
        printf("Starting squid failed using this command: %s\n", start);
        test = g_strdup_printf("%s -f %s 2>&1", SQUID_BINARY, SQUID_CONF);
	run_command_show_err(test);
	g_free(test);
	
	/* No sence in starting a malfunctioning server at system boot */
	init_stop(widgets);
    }
    else
      init_start(widgets);    

    g_free(start);
}
