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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gettext.h"
#include "widgets.h"
#include "create_user_settings.h"
#include "populate_shell_combo.h"
#include "make_settings_buttons.h"
#include "make_settings_entries.h"
#include "make_settings_combos.h"
#include "make_settings_spinbuttons.h"
#include "make_settings_checkbuttons.h"
#include "delete_user.h"
#include "add_user.h"
#include "apply_user.h"
#include "functions.h"



void create_user_settings(struct w *widgets)
{
    GtkTooltip *tooltips;
    GtkWidget *hbuttonbox;
    GtkWidget *delete_user_button;
    GtkWidget *add_user_button;
    GtkWidget *apply_user_button;
    int a = 0;
    int b = 1;
    int ent  = 0;
    int comb = 0;

    //tooltips = gtk_tooltips_new();
    hbuttonbox = gtk_hbutton_box_new();

    /* The delete and apply user settings buttons */
    delete_user_button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
    add_user_button = gtk_button_new_from_stock(GTK_STOCK_ADD);
    apply_user_button = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_box_pack_start(GTK_BOX(hbuttonbox), delete_user_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbuttonbox), add_user_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbuttonbox), apply_user_button, TRUE, TRUE, 0);


    g_signal_connect_swapped(G_OBJECT(delete_user_button), "clicked",
			     G_CALLBACK(delete_user), widgets);

    g_signal_connect_swapped(G_OBJECT(apply_user_button), "clicked",
			     G_CALLBACK(apply_user), widgets);

    g_signal_connect_swapped(G_OBJECT(add_user_button), "clicked",
			     G_CALLBACK(add_user), widgets);
			     
    gtk_table_attach(GTK_TABLE(widgets->usr_set_table), hbuttonbox, 0, 3, a, b, 0, 0, 5, 5);
    a++; b++;


    /* Username */	     /* Max length and input 350 chars */
    GtkWidget *username_button;
    widgets->user_set_entry[ent] = gtk_entry_new();
    username_button = make_button_with_entry(GTK_TABLE(widgets->usr_set_table),
                        		     widgets->user_set_entry[ent],
                    			     "gtk-refresh",
					     "Username:",
					     "Crate a randomized username",
					     0,1,a,b,350);
    a++; b++; ent++;

    g_signal_connect_swapped(G_OBJECT(username_button), "clicked",
			     G_CALLBACK(randomize_username), widgets);

    /* Password  */
    GtkWidget *password_button;
    widgets->user_set_entry[ent] = gtk_entry_new();
    password_button = make_button_with_entry(GTK_TABLE(widgets->usr_set_table),
                        		     widgets->user_set_entry[ent],
                    			     "gtk-refresh",
					     "Password:",
					     "Create a randomized password",
					     0,1,a,b,350);
    a++; b++; ent++;

    g_signal_connect_swapped(G_OBJECT(password_button), "clicked",
			     G_CALLBACK(randomize_password), widgets);


    /* Group */
    widgets->user_set_entry[ent] = make_entry_with_label(GTK_TABLE(widgets->usr_set_table), _(" Group: "), 0,1,a,b,350);
    gtk_widget_set_tooltip_text(widgets->user_set_entry[ent], "The group the user belongs to");
    a++; b++; ent++;


    /* Comment */
    widgets->user_set_entry[ent] = make_entry_with_label(GTK_TABLE(widgets->usr_set_table), _(" Comment: "), 0,1,a,b,350);
    gtk_widget_set_tooltip_text(widgets->user_set_entry[ent], "A comment about the user");
    a++; b++; ent++;


    /* Home directory */
    GtkWidget *homedir_button;
    widgets->user_set_entry[ent] = gtk_entry_new();
    homedir_button = make_button_with_entry(GTK_TABLE(widgets->usr_set_table),
                        		     widgets->user_set_entry[ent],
                    			     "gtk-refresh",
					     "Home directory:",
					     "Choose a home directory for this user",
					     0,1,a,b,350);
    a++; b++; ent++;

    /* Shell combo */    
    widgets->user_set_combo[comb] = make_label_textcombo_label(GTK_TABLE(widgets->usr_set_table), _(" Shell: "), 0,1,a,b, 80);
    populate_shell_combo(widgets->user_set_combo[comb]);
    a++; b++; comb++;

    gtk_widget_show_all(widgets->main_window);
}
