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
#include "create_disc_tab.h"
#include "populate_disc_tab.h"


/* Wether or not to let the disc section expand */
#define EXPAND_DISC_SECTION TRUE



void create_disc_tab(struct w *widgets)
{
    GtkCellRenderer *disc_cell_renderer;
    GtkWidget *disc_treeview_hbox;
    GtkWidget *disc_scrolled_window;
    gchar *utf8=NULL;

    GtkTooltip *tooltips;
    //tooltips = gtk_tooltips_new();

    /* Create the disc treeview in a scrolled window */
    disc_treeview_hbox = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox5), disc_treeview_hbox, EXPAND_DISC_SECTION, TRUE, 0);

    disc_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start(GTK_BOX(disc_treeview_hbox), disc_scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(disc_scrolled_window), 
						    GTK_POLICY_AUTOMATIC, 
						    GTK_POLICY_ALWAYS);

    widgets->disc_store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
    G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    widgets->disc_treeview = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(widgets->disc_treeview), GTK_TREE_MODEL(widgets->disc_store));

    gtk_container_add(GTK_CONTAINER(disc_scrolled_window), widgets->disc_treeview);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(widgets->disc_treeview), TRUE);

    /* Set the column labels in the treeview */
    disc_cell_renderer = gtk_cell_renderer_text_new();

    GtkTreeViewColumn *mount_col = gtk_tree_view_column_new_with_attributes(_("Mounted on"), disc_cell_renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->disc_treeview), GTK_TREE_VIEW_COLUMN(mount_col));

    GtkTreeViewColumn *free_space_col = gtk_tree_view_column_new_with_attributes(_("Free space"), disc_cell_renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->disc_treeview), GTK_TREE_VIEW_COLUMN(free_space_col));

    GtkTreeViewColumn *used_space_col = gtk_tree_view_column_new_with_attributes(_("Used space"), disc_cell_renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->disc_treeview), GTK_TREE_VIEW_COLUMN(used_space_col));

    GtkTreeViewColumn *total_space_col = gtk_tree_view_column_new_with_attributes(_("Total space"), disc_cell_renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->disc_treeview), GTK_TREE_VIEW_COLUMN(total_space_col));

    GtkTreeViewColumn *used_percent_col = gtk_tree_view_column_new_with_attributes(_("Percent used"), disc_cell_renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->disc_treeview), GTK_TREE_VIEW_COLUMN(used_percent_col));

    GtkTreeViewColumn *device_col = gtk_tree_view_column_new_with_attributes(_("Device"), disc_cell_renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->disc_treeview), GTK_TREE_VIEW_COLUMN(device_col));

    /* The update disc button */
    GtkWidget *disc_button_box = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox5), disc_button_box, FALSE, FALSE, 0);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(disc_button_box), GTK_BUTTONBOX_SPREAD);

    GtkWidget *update_disc_button = gtk_button_new_from_stock(GTK_STOCK_REFRESH);
    gtk_box_pack_start(GTK_BOX(disc_button_box), update_disc_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(update_disc_button), "clicked",
                             G_CALLBACK(populate_discs), widgets);

    gtk_widget_show_all(widgets->main_window);

    if( utf8 != NULL )
      g_free(utf8);
}
