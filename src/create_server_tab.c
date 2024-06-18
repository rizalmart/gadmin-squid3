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
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "widgets.h"
#include "gettext.h"
//#include "server_treeview_row_clicked.h"
#include "apply_server_settings.h"
#include "show_info.h"
//#include "create_import_window.h"

/* Wether or not to let the server section expand */
#define EXPAND_SERVER_SECTION FALSE

/* Wether or not to let the server settings section expand */
#define EXPAND_SERVER_SETTINGS_SECTION TRUE

#define NUM_SERVER_CELLS 3


void server_scroll_to_last_row(GtkTreeView *treeview, GtkTreeIter iter)
{
    /* Scroll to the last row */
    GtkTreeModel *model;
    GtkTreePath *path;
    GtkTreeSelection *selection;
                
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    path = gtk_tree_model_get_path(GTK_TREE_MODEL(model), &iter);
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    gtk_tree_selection_select_path(GTK_TREE_SELECTION(selection), path);
    gtk_tree_view_set_cursor(GTK_TREE_VIEW(treeview), path, 0, TRUE);

    if( path!=NULL )
      gtk_tree_path_free(path);
}

void add_server(struct w *widgets)
{
    GtkTreeIter iter;
    gchar *info;

    gtk_list_store_append(GTK_LIST_STORE(widgets->server_store), &iter);
    server_scroll_to_last_row(GTK_TREE_VIEW(widgets->server_treeview), iter);

    info = g_strdup_printf("%s",
    _("A new server row has been added.\nClick the cells in this row to write a new server specification.\nPress apply when the configuration is complete.\n"));
    show_info(info);
    g_free(info);
}

void del_server(gpointer data, GtkTreeView *treeview)
{
    /* Deletes a row in a treeview */
    GtkTreeModel *model;
    GtkTreeStore *model_store;
    GtkTreeSelection *select;
    GtkTreeIter iter;
    gboolean val;

    model_store = (GtkTreeStore *) gtk_tree_view_get_model(treeview);
    select = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    if( gtk_tree_selection_get_selected(select, &model, &iter) )
    {
	val = gtk_list_store_remove(GTK_LIST_STORE(model_store), &iter);
    }
}

void server_text_cell_edited(GtkCellRendererText *cell, gchar *path_string,
                        gchar *new_text, GtkListStore *store)
{
    /* Changes the edited cells text */
    GtkTreeIter iter;
    GtkTreePath *path = NULL;
    int column = (gint)  (glong) g_object_get_data(G_OBJECT(cell), "column");

    path = gtk_tree_path_new_from_string(path_string);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);

    if( path!=NULL )
      gtk_tree_path_free(path);

    gtk_list_store_set(GTK_LIST_STORE(store), &iter, column, new_text, -1);
}



void create_server_tab(struct w *widgets)
{
    GtkWidget *server_treeview_hbox, *settings_treeview_hbox;
    GtkWidget *server_scrolled_window;
    GtkWidget *frame;
    gchar *utf8=NULL;
//    GtkWidget *import_alignment;
//    GtkWidget *import_hbox;
//    GtkWidget *import_image;
//    GtkWidget *import_label;

    GtkCellRenderer *text_cell[NUM_SERVER_CELLS];
    int i = 0;

    GtkTooltip *tooltips;
    //tooltips = gtk_tooltips_new();
    

    /* Create the domain treeview in a scrolled window */
    server_treeview_hbox = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox1), server_treeview_hbox, EXPAND_SERVER_SECTION, TRUE, 0);

    server_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start(GTK_BOX(server_treeview_hbox), server_scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(server_scrolled_window), 
						    GTK_POLICY_AUTOMATIC, 
						    GTK_POLICY_ALWAYS);
    /* Must set a larger size or it wont scroll */
    gtk_widget_set_size_request(server_scrolled_window, -1, 100);

    widgets->server_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    widgets->server_treeview = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(widgets->server_treeview), GTK_TREE_MODEL(widgets->server_store));

    gtk_container_add(GTK_CONTAINER(server_scrolled_window), widgets->server_treeview);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(widgets->server_treeview), TRUE);

    /* Create text cells and set the column labels in the treeview */
    for(i=0; i<= NUM_SERVER_CELLS; i++)
    {
	text_cell[i] = gtk_cell_renderer_text_new();
    }

    i = 0;

    GtkTreeViewColumn *address_col = gtk_tree_view_column_new_with_attributes(_("Server address"), text_cell[i], "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->server_treeview), GTK_TREE_VIEW_COLUMN(address_col));
    i++;

    GtkTreeViewColumn *port_col = gtk_tree_view_column_new_with_attributes(_("Server port"), text_cell[i], "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->server_treeview), GTK_TREE_VIEW_COLUMN(port_col));
    i++;

//    GtkTreeViewColumn *type_col = gtk_tree_view_column_new_with_attributes(_("Server Type"), text_cell[i], "text", 2, NULL);
//    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->server_treeview), GTK_TREE_VIEW_COLUMN(type_col));
//    i++;

    GtkTreeViewColumn *cert_col = gtk_tree_view_column_new_with_attributes(_("HTTPS Server certificate"), text_cell[i], "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->server_treeview), GTK_TREE_VIEW_COLUMN(cert_col));
    i++;

//  Cant use with editable cells  g_signal_connect((gpointer)widgets->server_treeview, "button_press_event",  
//		      G_CALLBACK(server_treeview_row_clicked), widgets);


    /* The Delete Add and Apply server buttons */
    GtkWidget *server_button_box = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox1), server_button_box, FALSE, FALSE, 0);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(server_button_box), GTK_BUTTONBOX_SPREAD);

    /* The import users button with its non stock label and image */
//    GtkWidget *import_users_button = gtk_button_new();
//    gtk_box_pack_start(GTK_BOX(server_button_box), import_users_button, FALSE, FALSE, 0);
//    import_alignment = gtk_alignment_new (0.5, 0.5, 0, 0);
//    gtk_widget_show (import_alignment);
//    gtk_container_add(GTK_CONTAINER(import_users_button), import_alignment);
//    import_hbox = gtk_hbox_new (FALSE, 2);
//    gtk_widget_show (import_hbox);
//    gtk_container_add(GTK_CONTAINER(import_alignment), import_hbox);
//    import_image = gtk_image_new_from_stock("gtk-convert", GTK_ICON_SIZE_BUTTON);
//    gtk_widget_show (import_image);
//    gtk_box_pack_start(GTK_BOX(import_hbox), import_image, FALSE, FALSE, 0);
//    import_label = gtk_label_new_with_mnemonic(_("Import users"));
//    gtk_widget_show (import_label);
//    gtk_box_pack_start(GTK_BOX(import_hbox), import_label, FALSE, FALSE, 0);
//    gtk_label_set_justify(GTK_LABEL(import_label), GTK_JUSTIFY_LEFT);
//    g_signal_connect_swapped(G_OBJECT(import_users_button), "clicked", 
//                             G_CALLBACK(create_import_window), widgets);

    /* Delete server button */
    GtkWidget *delete_server_button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
    gtk_box_pack_start(GTK_BOX(server_button_box), delete_server_button, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(delete_server_button), "clicked", 
                             G_CALLBACK(del_server), widgets->server_treeview);
    /* Add server button */
    GtkWidget *add_server_button = gtk_button_new_from_stock(GTK_STOCK_ADD);
    gtk_box_pack_start(GTK_BOX(server_button_box), add_server_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(add_server_button), "clicked", 
                             G_CALLBACK(add_server), widgets);
    /* Apply server button */
    GtkWidget *apply_server_button = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_box_pack_start(GTK_BOX(server_button_box), apply_server_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(apply_server_button), "clicked", 
                             G_CALLBACK(apply_server_settings), widgets);

    /* Make the server cells editable */
    for(i=0; i< NUM_SERVER_CELLS; i++)
    {
	g_object_set(G_OBJECT(text_cell[i]), "editable", TRUE, NULL);
	g_signal_connect(text_cell[i], "edited",  
            G_CALLBACK(server_text_cell_edited), widgets->server_store);
    }

    /* Set column information on the cells */
    g_object_set_data(G_OBJECT(text_cell[0]), "column", (void*)(0));
    g_object_set_data(G_OBJECT(text_cell[1]), "column", (void*)(1));
    g_object_set_data(G_OBJECT(text_cell[2]), "column", (void*)(2));



    /* Server settings begin... */

    /* Create the server settings in a table inside a scrolled window */
    settings_treeview_hbox = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox1), settings_treeview_hbox, EXPAND_SERVER_SETTINGS_SECTION, TRUE, 0);

    widgets->server_settings_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start(GTK_BOX(settings_treeview_hbox), widgets->server_settings_scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(widgets->server_settings_scrolled_window), 
						    GTK_POLICY_AUTOMATIC, 
						    GTK_POLICY_ALWAYS);

    /* Must set a larger size or it wont scroll */
    gtk_widget_set_size_request(widgets->server_settings_scrolled_window, -1, 100);

    /* Add a vbox to the scrolled window */
    widgets->server_settings_vbox = gtk_vbox_new(TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(widgets->server_settings_scrolled_window), widgets->server_settings_vbox);

    /* Add a frame */
    frame = gtk_frame_new(_("Server settings"));

    /* Add a table with for the settings with 3 columns */
    widgets->srv_set_table = gtk_table_new(NUM_SERVERTAB_ENTRIES+NUM_SERVERTAB_SPINBUTTONS+NUM_SERVERTAB_COMBOS, 3, FALSE);
    gtk_box_pack_start(GTK_BOX(widgets->server_settings_vbox), frame, TRUE, TRUE, 1);
    gtk_container_add(GTK_CONTAINER(frame), widgets->srv_set_table);


    gtk_widget_show_all(widgets->main_window);

    if( utf8 != NULL )
      g_free(utf8);
}
