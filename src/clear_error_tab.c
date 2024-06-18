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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "gettext.h"
#include "widgets.h"
#include "allocate.h"
#include "show_info.h"
#include "clear_error_tab.h"
#include "populate_error_tab.h"



void clear_error_tab(struct w *widgets)
{
    /* Clears the error/syslog */
    FILE *fp;
    char *line, *new_syslog;
    long file_size = 0;
    gchar *info;

    /* Collect every line in syslog thats not referring to squid */
    if((fp=fopen(ERROR_LOG, "r"))==NULL)
    {
        info = g_strdup_printf(_("Couldnt open system log here: %s\n"), ERROR_LOG);
        show_info(info);
        g_free(info);
        return;
    }
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    line = allocate(file_size+16384);
    new_syslog = allocate(file_size+16384);

    if( file_size > 1 )
    while(fgets(line, file_size, fp)!=NULL)
      if( ! strstr(line, "squid") )
        strcat(new_syslog, line);

    fclose(fp);    
    free(line);

    /* Write the modified syslog */
    if((fp=fopen(ERROR_LOG, "w+"))==NULL)
    {
        info = g_strdup_printf(_("Could not write system log here: %s\n"), ERROR_LOG);
        show_info(info);
        g_free(info);
	free(new_syslog);
        return;
    }
    fputs(new_syslog, fp);
    free(new_syslog);
    fclose(fp);    

    populate_error_tab(widgets);
}
