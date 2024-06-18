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
#include <string.h>
#include "widgets.h"
#include "gettext.h"
#include "create_access_tab.h"
#include "populate_access_tab.h"
#include "clear_access_tab.h"


/* Wether or not to let the file section expand */
#define EXPAND_FILE_SECTION TRUE



void create_access_tab(struct w *widgets)
{
    GtkCellRenderer *file_cell_renderer;
    GtkWidget *file_treeview_hbox;
    GtkWidget *file_scrolled_window;
    gchar *utf8=NULL;

    GtkTooltip *tooltips;
    //tooltips = gtk_tooltips_new();

    /* Create the file treeview in a scrolled window */
    file_treeview_hbox = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox6), file_treeview_hbox, EXPAND_FILE_SECTION, TRUE, 0);

    file_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start(GTK_BOX(file_treeview_hbox), file_scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(file_scrolled_window), 
						    GTK_POLICY_AUTOMATIC, 
						    GTK_POLICY_ALWAYS);

    widgets->file_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    widgets->file_treeview = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(widgets->file_treeview), GTK_TREE_MODEL(widgets->file_store));

    gtk_container_add(GTK_CONTAINER(file_scrolled_window), widgets->file_treeview);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(widgets->file_treeview), TRUE);

    /* Set the column labels in the treeview */
    file_cell_renderer = gtk_cell_renderer_text_new();

    GtkTreeViewColumn *filename_col = gtk_tree_view_column_new_with_attributes(_("Downloaded content"), file_cell_renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->file_treeview), GTK_TREE_VIEW_COLUMN(filename_col));

    GtkTreeViewColumn *client_addr_col = gtk_tree_view_column_new_with_attributes(_("Client address"), file_cell_renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->file_treeview), GTK_TREE_VIEW_COLUMN(client_addr_col));

    GtkTreeViewColumn *target_addr_col = gtk_tree_view_column_new_with_attributes(_("Target address"), file_cell_renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->file_treeview), GTK_TREE_VIEW_COLUMN(target_addr_col));

/*
    GtkTreeViewColumn *filename_col = gtk_tree_view_column_new_with_attributes(_("Files"), file_cell_renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->file_treeview), GTK_TREE_VIEW_COLUMN(filename_col));

    GtkTreeViewColumn *hostname_col = gtk_tree_view_column_new_with_attributes(_("Hostname"), file_cell_renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->file_treeview), GTK_TREE_VIEW_COLUMN(hostname_col));

    GtkTreeViewColumn *sot_col = gtk_tree_view_column_new_with_attributes(_("Start of transfer"), file_cell_renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->file_treeview), GTK_TREE_VIEW_COLUMN(sot_col));
*/

    /* The update file button */
    GtkWidget *file_button_box = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox6), file_button_box, FALSE, FALSE, 0);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(file_button_box), GTK_BUTTONBOX_SPREAD);

    GtkWidget *clear_file_button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
    gtk_box_pack_start(GTK_BOX(file_button_box), clear_file_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(clear_file_button), "clicked",
                             G_CALLBACK(clear_access_tab), widgets);

    GtkWidget *update_file_button = gtk_button_new_from_stock(GTK_STOCK_REFRESH);
    gtk_box_pack_start(GTK_BOX(file_button_box), update_file_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(update_file_button), "clicked",
                             G_CALLBACK(populate_access_tab), widgets);

    gtk_widget_show_all(widgets->main_window);

    if( utf8 != NULL )
      g_free(utf8);
}
