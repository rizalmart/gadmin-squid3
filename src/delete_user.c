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
//#include "support.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "widgets.h"
#include "gettext.h"
#include "allocate.h"
#include "commands.h"
#include "show_info.h"
#include "delete_user.h"
#include "functions.h"
#include "system_defines.h"
#include "create_del_system_user_question.h"
#include "populate_users.h"
#include "populate_user_settings.h"
#include "populate_conf_tab.h"
#include "select_first_user.h"
#include "reread_conf.h"


#ifdef USE_DARWIN
#include "osx_functions.h"
#endif



extern char global_server_address[1024];
extern char global_server_port[1024];
extern char global_server_type[1024];
extern char global_user_name[1024];

extern int activated;



void delete_user(struct w *widgets)
{
    /*  Deletes the user settings from the selected server and
        asks to delete the system user as well */
    FILE *fp;
    long conf_size;
    char *config;

    char *user_settings;
    char *UserUsername, *AllowuserName;
    char *old_buffer, *address_buffer, *port_buffer;
    int length=0, user_deleted=0, found_server=0;
    G_CONST_RETURN gchar *username;
    gchar *info;


return; // For now

    username = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[0]));

    /* If the namefield in username is empty inform that we refuse to do anything */
    length = strlen(username);
    if( length == 0 ) 
    {
	info = g_strdup_printf(_("Could not delete the user, select a user first.\n"));
        show_info(info);
        g_free(info);
        return;
    }

    /* Dont ever delete the root user, it cant be added in the gui but could be added by the user */
    if( username[0]=='r' && username[1]=='o' && username[2]=='o' && username[3]=='t' && strlen(username)==4 ) 
    {
	info = g_strdup_printf(_("User root can not be deleted.\n"));
        show_info(info);
        g_free(info);
    	return;
    }

    /* Delete the configuration for the deleted user in the correct server */
    if((fp=fopen(SQUID_CONF, "r"))==NULL)
    {
	info = g_strdup_printf(_("squid.conf was not found here:\n%s\n"), SQUID_CONF);
        show_info(info);
        g_free(info);
        return;
    }

    fseek(fp, 0, SEEK_END);
    conf_size = ftell(fp);
    rewind(fp);


    UserUsername = allocate(4096);
    strcpy(UserUsername, "User ");
    strcat(UserUsername, username);
    /* Avoid similar user deletion */
    strcat(UserUsername, "\n");
    
    AllowuserName = allocate(4096);
    strcpy(AllowuserName, "AllowUser ");
    strcat(AllowuserName, username);
    /* Avoid similar user deletion */
    strcat(AllowuserName, "\n");
    
    config = allocate(conf_size); 

    old_buffer = allocate(conf_size);
    user_settings = allocate(conf_size);

    address_buffer = allocate(8192+15);
    port_buffer = allocate(8192+3);               


    /* Get everything except the selected users settings and allowuser in the selected server */

    if( strstr((char *)global_server_type, "Virtualhost") )
       snprintf(address_buffer, 4000, "<VirtualHost %s>", global_server_address);
    else
       snprintf(address_buffer, 4000, "%s", global_server_address);
     
    snprintf(port_buffer, 4000, "Port %s", global_server_port);


    /* Standard server selected, delete the selected user in this (first) server */
    if( ! strstr((char *)global_server_type, "Virtualhost") )
      found_server = 1;


    if( conf_size > 1 )
    while(fgets(old_buffer, conf_size, fp)!=NULL)
    {
	/* Is this the correct server to delete the user from.. */
        if( strstr("Virtualhost", (char *)global_server_type) && ! found_server
	&&  strstr(old_buffer, address_buffer) )
	{
	    strcat(config, old_buffer);

	    /* Lets see if its the same port as the selected one */
	    /* If this server has the same port its the correct server .. the end. */
	    while(fgets(old_buffer, conf_size, fp)!=NULL)
	    {
		if( strlen(old_buffer) > 8000 )
		  continue;

		/* This will expect the servers port on the second line ! 
		 * else itll miss some values .. */
		if( strstr(old_buffer, "Port") && strstr(old_buffer, port_buffer) )
		{
		    found_server = 1;

		    strcat(config, old_buffer);
		    break;
		}
		else
		  strcat(config, old_buffer);

		if( strstr(old_buffer, "</Virtualhost>") )
		  break;
	    }
	}
	else
	  strcat(config, old_buffer);
	 

	/* Continue until we find the selected server */
	if( found_server )
	  break;
    }


    if( ! found_server )
    {
	fclose(fp);
	free(old_buffer);
	free(address_buffer);
	free(port_buffer);
	free(config);
	free(user_settings);
	free(UserUsername);
	free(AllowuserName);

	info = g_strdup_printf(_("Could not find the server, user not deleted\n"));
        show_info(info);
        g_free(info);
	return;
    }


    /* We have collected everything up to  
     * <Virtualhost HOSTNAME>
     * Port PORTNR */
    if( conf_size > 1 )
    while(fgets(old_buffer, conf_size, fp) !=NULL)
    {
        /* Skip the Allowuser line in the LIMIT context */
	if( strstr(old_buffer, AllowuserName) )
	  continue;

	/* If this is this users settings, dont copy it */
        if( strstr(old_buffer, "<Anonymous ") && ! user_deleted )
        {
            strcpy(user_settings, old_buffer);        


	    while(fgets(old_buffer, conf_size, fp)!=NULL)
	    {
        	strcat(user_settings, old_buffer);

		/* Is this the settings to delete.. */
		if( strstr(old_buffer, UserUsername) && ! strstr(old_buffer, "DirFakeUser") )
		  user_deleted=1;
		
		if( strstr(old_buffer, "</Anonymous>") )
		  break;
	    }

	    /* Get every other users settings */	    
            if( ! user_deleted )
              strcat(config, user_settings);
        }
    	else
	  {
	     /* Delete extra whitespace between the users settings and
	      * get everything but the this users settings. */
	      
	     if( strlen(old_buffer) > 3 )
	       strcat(config, old_buffer);
	  }
    } 	  
    fclose(fp);
    free(old_buffer);
    free(address_buffer);
    free(port_buffer);
    free(UserUsername);
    free(AllowuserName);
    free(user_settings);

    /* Write the new config without the deleted users settings */
    if((fp=fopen(SQUID_CONF, "w+"))==NULL)
    {
        printf("Error Writing configuration here: %s\n", SQUID_CONF);
        return;
    }
    fputs(config, fp);
    fclose(fp);
    free(config);


    fix_newlines_in_conf();


    if( user_exists(username) )
    {
	create_del_system_user_question(widgets);
	gtk_widget_show_all(widgets->del_system_user_question_window);
    }
    else
      {
	  /* Update the user list and the user settings */
	  populate_users(widgets);

	  select_first_user(widgets);

	  populate_user_settings(widgets);
    
	  populate_conf_tab(widgets);
      }

    /* Update the server */
    reread_conf(widgets);
}


void dont_delete_system_user(struct w *widgets)
{
    gtk_widget_destroy(widgets->del_system_user_question_window);

    /* Update the user list and the user settings */
    populate_users(widgets);

    select_first_user(widgets);

    populate_user_settings(widgets);
    
    populate_conf_tab(widgets);

    /* Update the server */
    reread_conf(widgets);
}


void do_delete_system_user(GtkButton *button, struct w *widgets)
{
    /* Deletes the system user */

#ifndef USE_DARWIN
    /* Avoid unused warning */
    char *command;
#endif

    int deleted = 0;
    int length  = 0;
    gchar *utf8 = NULL;
    gchar *info;
    char *username;

    gtk_widget_destroy(widgets->del_system_user_question_window);


    username = allocate(1024);
    snprintf(username, 1000, "%s", global_user_name);

    if( username == NULL )
    {
	info = g_strdup_printf(_("No username specified.\n"));
	show_info(info);
	g_free(info);

	return;
    }


    /* If the username field is empty inform that this cant be done. */
    length = strlen(username);
    if( length == 0 ) 
    {
	info = g_strdup_printf(_("No username specified.\n"));
	show_info(info);
	g_free(info);

    	return;
    }

    if( username[0]=='r' && username[1]=='o' && username[2]=='o' && username[3]=='t' && strlen(username) == 4 ) 
    {
	info = g_strdup_printf(_("The root user can not be deleted.\n"));
	show_info(info);
	g_free(info);

    	return;
    }





#ifndef USE_DARWIN
    if( user_exists(username) )
    {
	command = allocate(4096);
	snprintf(command, 4000, "%s %s", DELUSER, username);
	if( run_command(command) )
	    deleted = 1;
	
	free(command);
    }
#elif USE_DARWIN
    if( niutil_user_exists(username) )
    {
	if( niutil_userdel(username) )
	  deleted = 1;
    }
#endif




    if( ! deleted )
    {
	info = g_strdup_printf(_("Failed deleting user: %s\n"), username);
	show_info(info);
	g_free(info);
    }

    if( utf8!=NULL )
      g_free(utf8);


    /* Update the user list and the user settings */
    populate_users(widgets);

    select_first_user(widgets);

    populate_user_settings(widgets);
    
    populate_conf_tab(widgets);

    /* Update the server */
    reread_conf(widgets);
}
