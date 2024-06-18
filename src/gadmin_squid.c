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
#include "gettext.h"
#include "support.h"
#include "create_main_window.h"
#include "gadmin_squid.h"
#include "widgets.h"
#include "commands.h"
#include "functions.h"
#include "status_update.h"
#include "set_version.h"
#include "standard_conf.h"

/* For getuid */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "create_server_tab.h"
#include "create_server_settings.h"
#include "create_acl_tab.h"
#include "create_user_tab.h"
#include "create_user_settings.h"
#include "create_transfer_tab.h"
#include "create_disc_tab.h"
#include "create_access_tab.h"
#include "create_error_tab.h"
#include "create_cache_tab.h"
#include "create_conf_tab.h"
#include "populate_gadmin_squid.h"
#include "add_standard_users.h"
#include "create_swap_dirs.h"
#include "show_info.h"

int MAX_READ_POPEN = 16384;
int activated = 0;
int use_tls = 0;

int info_window_exit_main = 0;
int squid_mayor_version = 0;

char global_server_address[1024]="";
char global_server_port[1024]="";
char global_server_name[1024]="";
char global_server_type[1024]="";
char global_user_name[1024]="";



int main(int argc, char *argv[])
{
    gchar *conf_path, *info;
    int i = 0;

#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

    //gtk_set_locale();
    gtk_init(&argc, &argv);

    wid* widgets = g_malloc(sizeof(wid));

    /* Non root usage */
    if( ! getuid() == 0 )
    {
	/* For setting a different exit method in the info window */
	info_window_exit_main = 1;

        info = g_strdup_printf("You must be root to run: %s\nThis window will close in 10 seconds.\n", PACKAGE);
        show_info(info);
        g_free(info);

	/* Users can close the info window earlier then the timeout */
	for(i=0; i<10; i++)
	{
    	    while(gtk_events_pending())
                  gtk_main_iteration();

	    /* Set when close info window is clicked */
	    if( info_window_exit_main == 2 )
	      break;

    	    usleep(100000*10);
    	}

	g_free(widgets);
	return 0;
    }


    /* Create the main window */
    create_main_window(widgets);

    /* Set SQUID Version */
    set_version(widgets);

    /* If squid has the mod_tls module we 
       create, show and use the TLS widgets */
    use_tls = using_tls();

    /* Create the server tab */
    create_server_tab(widgets);

    /* Create the server settings */
    create_server_settings(widgets);

    /* Create the ACL tab */
    create_acl_tab(widgets);

    /* Create the user tab */
    create_user_tab(widgets);

    /* Create the user settings */
    create_user_settings(widgets);

    /* Create the transfer tab. Change name */
    create_transfer_tab(widgets);

    /* Create the disc tab */
    create_disc_tab(widgets);

    /* Create the access tab */
    create_access_tab(widgets);

    /* Create the error tab */
    create_error_tab(widgets);

    /* Create the cache tab */
    create_cache_tab(widgets);

    /* Create the conf tab */
    create_conf_tab(widgets);

    /* Create the application configuration directory. */
    make_dir_chmod(APPCONFDIR, "0700");


    conf_path = g_strdup_printf("%s", SQUID_CONF);

    /* If theres no squid.conf, add one */
    if( ! file_exists(conf_path) )
      add_standard_conf(widgets);
    else
    /* If squid.conf is bad, ask to add a new one */
    if( ! conf_ok(conf_path) )
    {
//	create_swap = 0;
        create_standard_conf_question(widgets);
    }
    else
      /* The conf is ok, populate the entire gui */
      populate_gadmin_squid(widgets);

    g_free(conf_path);


    /* Backup the configuration */
    backup_configuration(widgets);


    /* Create standard squid user and group if they dont exist */
    add_standard_users();

    /* Create squids swap directories if they dont exist */
    create_swap_dirs(widgets);



    /* Window close button (x) */
    g_signal_connect(G_OBJECT(widgets->main_window), "destroy",
	             G_CALLBACK(gtk_main_quit), NULL);


    g_timeout_add(1000, (GSourceFunc) status_update, widgets);

    gtk_main();

    g_free(widgets);


    return 0;
}
