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
#include "allocate.h"
#include "commands.h"
#include "show_info.h"
#include "add_user.h"
#include "functions.h"
#include "system_defines.h"
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



void add_user(struct w *widgets)
{
    /* Adds a new system user */
    int length=0, user_added=0;
    gchar *utf8=NULL;
    gchar *info, *cmd;

    G_CONST_RETURN gchar *username;
    G_CONST_RETURN gchar *password;
    G_CONST_RETURN gchar *group;
    G_CONST_RETURN gchar *homedir;
    G_CONST_RETURN gchar *comment;
    G_CONST_RETURN gchar *shell;

    username  = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[0]));
    password  = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[1]));
    group     = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[2]));
    comment   = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[3]));
    homedir   = gtk_entry_get_text(GTK_ENTRY(widgets->user_set_entry[4]));
    
    /* Shell is a gtk_combo_box_entry_new_text */
    //shell     = gtk_entry_get_text(GTK_ENTRY(gtk_container_get_children(GTK_BIN(widgets->user_set_combo[0]))));

    /* If the shell is false it will add the users directory as /dev/null and the specified false shell.
     * If the shell is real it will add the users directory as /USERSHOME/username and the specified real shell. 
     */

    /* If the username field is empty inform that this cant be done. */
    length = strlen(username);
    if( length == 0 ) 
    {
	info = g_strdup_printf(_("You must specify a username.\n"));
	show_info(info);
	g_free(info);
    	return;
    }

    if( username[0]=='0'||username[0]=='1'||username[0]=='2'||username[0]=='3'||username[0]=='4' 
    ||  username[0]=='5'||username[0]=='6'||username[0]=='7'||username[0]=='8'||username[0]=='9') 
    {
	info = g_strdup_printf(_("Failed adding user: %s\nA user name can not begin with a number.\n"), username);
	show_info(info);
	g_free(info);
    	return;
    }

    if( username[0]=='r' && username[1]=='o' && username[2]=='o' && username[3]=='t' && strlen(username) == 4 ) 
    {
	info = g_strdup_printf(_("Failed adding user: %s\nThe root user can not be added for security reasons.\n"), username);
	show_info(info);
	g_free(info);
    	return;
    }

    if( strstr((char *)username, "<") || strstr((char *)username, ">") ) 
    {
	info = g_strdup_printf(_("Failed adding user: %s\nchars \"<\" and \">\" arent allowed.\n"), username);
	show_info(info);
	g_free(info);
    	return;
    }

    /* If the password field has less then 6 chars we inform that this cant be done */
    length = strlen(password);
    if( length < 6 ) 
    {
	info = g_strdup_printf(_("Failed adding user: %s\nA minimum password length of 6 chars is required.\n"), username);
	show_info(info);
	g_free(info);
    	return;
    }

    /* If the group field has less then 3 chars we inform that this cant be done */
    length = strlen(group);
    if( length == 0 )
    {
	info = g_strdup_printf(_("Failed adding user: %s\nNo group specified.\n"), username);
	show_info(info);
	g_free(info);
    	return;
    }

    /* If the shell field has less then 3 chars we inform that this cant be done */
    length = strlen(shell);
    if( length < 3 )
    {
	info = g_strdup_printf(_("Failed adding user: %s\nNo shell specified.\n"), username);
	show_info(info);
	g_free(info);
    	return;
    }

    /* If the directory field has 0 chars we inform that this cant be done */
    length = strlen(homedir);
    if( length < 8 )  /* /home/b /var/abc */
    {
	info = g_strdup_printf(_("Failed adding user: %s\nThe specified home directory is too short.\n"), username);
	show_info(info);
	g_free(info);
    	return;
    }

    /* A comment is required */
    length = strlen(comment);
    if( length == 0 ) 
    {
	info = g_strdup_printf(_("A comment is required to add a user.\n"));
	show_info(info);
	g_free(info);
    	return;
    }

    /* The selected shell is false, add the group and the user with a false shell and home /dev/null */
    if( strstr(shell, "nologin") || strstr(shell, "false") || strstr(shell, "dev/null") )
    {
	/* All supported systems but darwin */
#ifndef USE_DARWIN

	/* Add the group if it doesnt exist */
	if( ! group_exists(group) )
	{
	    cmd = g_strdup_printf("%s '%s'", ADDGROUP, group);
	    if( ! run_command(cmd) )
	    {
		info = g_strdup_printf(_("Error adding group: %s\n"), group);
 		show_info(info);
		g_free(info);
	    }
	    g_free(cmd);
	}

	/* Add the user to this group if it doesnt exist */
        if( ! user_exists(username) )
        {
	    cmd = g_strdup_printf("%s '%s' -g '%s' -d /dev/null -c '%s' -s %s", ADDUSER, username, group, comment, shell);
	    if( ! run_command(cmd) )
	    {
		info = g_strdup_printf(_("Failed adding user: %s\n"), username);
 		show_info(info);
		g_free(info);
	    }
	    else
	      user_added = 1;

	    g_free(cmd);
	}

#elif USE_DARWIN
	/* Add the false user using darwins niutil commands (Darwin is out of sync) */
	if( ! niutil_user_exists(username) )    
	{
	    if( ! niutil_useradd(username, shell) )
	    {
		info = g_strdup_printf(_("Failed adding user: %s\n"), username);
 		show_info(info);
		g_free(info);
	    }
	    else
	      user_added = 1;
	}
#endif
    }
    else 	
      {

         /* The selected shell is not false add a real user account */
#ifndef USE_DARWIN

	/* Add the group if it doesnt exist */
	if( ! group_exists(group) )
	{
	    cmd = g_strdup_printf("%s '%s'", ADDGROUP, group);
	    if( ! run_command(cmd) )
	    {
		info = g_strdup_printf(_("Failed adding group: %s\n"), group);
 		show_info(info);
		g_free(info);
	    }
	    g_free(cmd);
	}
	
	/* Add the user to this group if it doesnt exist */
	if( ! user_exists(username) )
	{
	    /* Add the user with a real shell to /USERSHOME/ UserName (was: -m -s) */
	    cmd = g_strdup_printf("%s '%s' -g '%s' -d '%s%s' -c '%s' -s %s", ADDUSER, username, group, USERSHOME, username, comment, shell);
	    if( ! run_command(cmd) )
	    {
		info = g_strdup_printf(_("Failed adding user: %s\n"), username);
 		show_info(info);
		g_free(info);
	    }
	    else
	      user_added = 1;

	    g_free(cmd);
	}

#elif USE_DARWIN
	 /* Add a real darwin user using the niutil commands (Darwin is out of sync) */
	 if( ! niutil_user_exists(username) )    
	 {
	     if( ! niutil_useradd(username, shell) )
	     {
		info = g_strdup_printf(_("Failed adding user: %s\n"), username);
 		show_info(info);
		g_free(info);
	     }
	     else
	       user_added = 1;
	 }
#endif
    }


    /* Dont add anything if we couldnt add the system user */
#ifndef USE_DARWIN
    if( ! user_exists(username) )
#elif USE_DARWIN
    if( ! niutil_user_exists(username) )
#endif
    {
	info = g_strdup_printf(_("The system user was not added because uppercase\nor language specific letters are not allowed.\n"));
 	show_info(info);
	g_free(info);
	return;
    }


    /* Password the user if it didnt exist before */
    if( user_added )
    {
#ifndef USE_DARWIN
         password_user(username, password);
#elif USE_DARWIN
	 niutil_password_user(username, password);
#endif
    }
    else
      {
	  info = g_strdup_printf(_("The system user \"%s\" already exists.\n"), username);
 	  show_info(info);
	  g_free(info);
      }

    /* Update the user list and the user settings */
    populate_users(widgets);
    select_first_user(widgets);

    populate_user_settings(widgets);
    
    if( utf8!=NULL )
      g_free(utf8);
}
