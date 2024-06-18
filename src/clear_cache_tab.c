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
#include "clear_cache_tab.h"
#include "populate_cache_tab.h"



void clear_cache_tab(struct w *widgets)
{
    /* Clears cache.log */
    FILE *fp;
    gchar *info;

    /* Clear cache.log */    
    if((fp=fopen(CACHE_LOG, "w+"))==NULL)
    {
        info = g_strdup_printf(_("Couldnt write to cache.log here: %s\n"), CACHE_LOG);
        show_info(info);
        g_free(info);
        return;
    }
    fclose(fp);

    populate_cache_tab(widgets);
}
