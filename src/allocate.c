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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * allocate(long allocate_size)
{
    char *ret;
    ret = malloc(allocate_size);
    if( ret == NULL )
    {
	printf("Cant allocate enough ram, exiting\n");
	exit(1);
    }
    memset(ret, 0, allocate_size);

    return ret;
}
