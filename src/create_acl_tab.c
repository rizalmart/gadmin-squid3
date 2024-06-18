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
#include "show_info.h"
//#include "server_treeview_row_clicked.h"
//#include "add_server.h"
//#include "delete_server.h"
#include "apply_server_settings.h"
//#include "create_import_window.h"

/* Wether to let the acl sections expand */
#define EXPAND_ACL_SECTION FALSE

/* Define number of treeviews */
#define NUM_ACL_TREEVIEWS 2

/* Define number of text/editable and combo/selectable cells */
#define NUM_TEXT_CELLS 2+1
#define NUM_COMBO_CELLS 4+1

/* The acl tab requires 2 hboxes per added section */
#define NUM_HBOXES NUM_ACL_TREEVIEWS * 2

GtkListStore *acl_combo_list[4];

char mod_type[1024]="";


void set_acl_type(char *type, struct w *widgets)
{
    snprintf(mod_type, 1000, "%s", type);
}

void scroll_to_last_row(GtkTreeView *treeview, GtkTreeIter iter)
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
    gtk_tree_path_free(path);
}

void add_acl(struct w *widgets)
{
    GtkTreeIter iter;
    gchar *info;

    if( ! strcmp(mod_type, "first_acl_treeview") )
    {
	gtk_list_store_append(GTK_LIST_STORE(widgets->acl_store), &iter);
	scroll_to_last_row(GTK_TREE_VIEW(widgets->acl_treeview), iter);
    }
    if( ! strcmp(mod_type, "second_acl_treeview") )
    {
	gtk_list_store_append(GTK_LIST_STORE(widgets->aclperm_store), &iter);
	scroll_to_last_row(GTK_TREE_VIEW(widgets->aclperm_treeview), iter);
    }

    info = g_strdup_printf("%s",
    _("A new row has been added for access control.\nClick the cells in this row to write a new access control specification.\n"));
    show_info(info);
    g_free(info);
}

void delete_row_clicked(gpointer data, GtkTreeView *treeview)
{
    /* Deletes the selected treeview row */
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

/* Populate acl_combo_list_3 in the second treeview
   with the contents of the first treeview's ACL Name list.
   Dont add duplicate ACL name entries. */
gboolean names_foreach(GtkTreeModel *model, GtkTreePath *path,
        		  GtkTreeIter *iter, char *unused)
{
    GtkTreeIter list_iter, new_iter;
    gchar *list_name, *acl_name;
    int acl_name_exists = 0;

    gtk_tree_model_get(model, iter, 0, &acl_name, -1);

    if( acl_name != NULL )
    {
	/* Check if this acl name has already been added to the combo */            
	if( gtk_tree_model_get_iter_first(GTK_TREE_MODEL(acl_combo_list[3]), &list_iter) )
        {
            do {
                    gtk_tree_model_get(GTK_TREE_MODEL(acl_combo_list[3]), &list_iter, 0, &list_name, -1);
                    if( list_name )
    		    {
                        if( strcmp(acl_name, list_name) == 0 )
                        {
			    acl_name_exists = 1;
			    break;
                        }
                    }
                    if( list_name!=NULL )
                      g_free(list_name);
               
	       }    while(gtk_tree_model_iter_next(GTK_TREE_MODEL(acl_combo_list[3]), &list_iter));
        }

	if( ! acl_name_exists )
	{
	    gtk_list_store_append(acl_combo_list[3], &new_iter);
	    gtk_list_store_set(acl_combo_list[3], &new_iter, 0, acl_name, -1);
	}

	g_free(acl_name);
    }

    return FALSE;
}

void populate_aclperm_name_list(GtkListStore *store)
{
    gchar *unused=NULL;
    GtkTreeIter iter;

    /* Clear the aclperm treeviews Name list combo */
    gtk_list_store_clear(GTK_LIST_STORE(acl_combo_list[3]));

    /* Squid-3.x. Has a built in 'all' rule so always add it first. */
    gtk_list_store_append(acl_combo_list[3], &iter);
    gtk_list_store_set(acl_combo_list[3], &iter, 0, "all", -1);
    
    /* Iterate the name column of the first acl treeview and append 
       the values to the aclperm name list in the second treeview. */
    gtk_tree_model_foreach(GTK_TREE_MODEL(store),
    (GtkTreeModelForeachFunc) names_foreach, unused);
}

void text_cell_edited(GtkCellRendererText *cell, gchar *path_string,
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

    /* Populate the second treeviews Name column if name
       is edited in the first treeview */
    if( column == 0 )
    {
	populate_aclperm_name_list(store);
    }
}


void combo_cell_edited(GtkCellRendererText *cell, gchar *path_string,
			gchar *new_text, GtkListStore *store)
{
    /* Changes cell combo text */
    GtkTreeIter iter;
    GtkTreePath *path = NULL;
    int column = (gint)  (glong) g_object_get_data(G_OBJECT(cell), "column");

    path = gtk_tree_path_new_from_string(path_string);
    gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);
    if( path!=NULL )
      gtk_tree_path_free(path);

    gtk_list_store_set(GTK_LIST_STORE(store), &iter, column, new_text, -1);
}



void create_acl_tab(struct w *widgets)
{
    GtkWidget *hbox[NUM_HBOXES];
    GtkTooltip *tooltips;
    gchar *utf8=NULL;
    int i = 0, z = 0;

    GtkWidget *acl_scrolled_window;
    GtkWidget *aclperm_scrolled_window;

    /* The first treeview has 2 text/editable cells */
    GtkCellRenderer *text_cell[NUM_TEXT_CELLS];
    /* The treeviews has a total of 4 combo cells */
    GtkCellRenderer *combo_cell[NUM_COMBO_CELLS];
    /* For the liststores combo contents */
    GtkTreeIter iter;

    //tooltips = gtk_tooltips_new();

    /* Pack hboxes into the vbox */
    for(i=0; i<NUM_HBOXES; i++)
    {
	hbox[i] = gtk_hbox_new(TRUE, 0);

	/* Button box containers should not expand */
	if( i == 1 || i == 3 )
	  gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox2), hbox[i], FALSE, TRUE, 0);
	else
	  gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox2), hbox[i], TRUE, TRUE, 0);
    }  

    /* Create text/editable cell renderers */
    for(i=0; i<NUM_TEXT_CELLS; i++)
    {
	text_cell[i] = gtk_cell_renderer_text_new();
	g_object_set(text_cell[i], "editable", TRUE, NULL);
    }
    /* Set column number on the cell */
    g_object_set_data(G_OBJECT(text_cell[0]), "column", (void*)(0));
    g_object_set_data(G_OBJECT(text_cell[1]), "column", (void*)(2));

    i = 0;

    /* ACL TREEVIEW BEGIN */
    acl_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(hbox[0]), acl_scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(acl_scrolled_window), 
						    GTK_POLICY_AUTOMATIC, 
						    GTK_POLICY_ALWAYS);
    /* Must set a larger size or it wont scroll */
    gtk_widget_set_size_request(acl_scrolled_window, -1, 100);

    widgets->acl_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    widgets->acl_treeview = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(widgets->acl_treeview), GTK_TREE_MODEL(widgets->acl_store));
    gtk_container_add(GTK_CONTAINER(acl_scrolled_window), widgets->acl_treeview);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(widgets->acl_treeview), TRUE);

    /* Create combo cell renderers */
    for(z=0; z<NUM_COMBO_CELLS; z++)
    {
	combo_cell[z] = gtk_cell_renderer_combo_new();
    }

    z = 0;

    /* Set the column labels in the treeview */
    GtkTreeViewColumn *aclname_col = gtk_tree_view_column_new_with_attributes(_("ACL Name"), text_cell[i], "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->acl_treeview), GTK_TREE_VIEW_COLUMN(aclname_col));
    i++;

    GtkTreeViewColumn *acltype_col = gtk_tree_view_column_new_with_attributes(_("ACL Type"), combo_cell[z], "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->acl_treeview), GTK_TREE_VIEW_COLUMN(acltype_col));
    z++;

    GtkTreeViewColumn *aclvalue_col = gtk_tree_view_column_new_with_attributes(_("ACL Value"), text_cell[i], "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->acl_treeview), GTK_TREE_VIEW_COLUMN(aclvalue_col));
    i++;

    /* ACL button box */
    GtkWidget *acl_button_box = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(hbox[1]), acl_button_box, FALSE, TRUE, 0);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(acl_button_box), GTK_BUTTONBOX_SPREAD);

    /* Delete button */
    GtkWidget *delete_acl_button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
    gtk_box_pack_start(GTK_BOX(acl_button_box), delete_acl_button, FALSE, FALSE, 0);
    g_signal_connect(delete_acl_button, "clicked",  
	      G_CALLBACK(delete_row_clicked), widgets->acl_treeview);
    /* Add button */
    GtkWidget *add_acl_button = gtk_button_new_from_stock(GTK_STOCK_ADD);
    gtk_box_pack_start(GTK_BOX(acl_button_box), add_acl_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(add_acl_button), "clicked", 
                             G_CALLBACK(set_acl_type), "first_acl_treeview");
    g_signal_connect_swapped(G_OBJECT(add_acl_button), "clicked", 
                             G_CALLBACK(add_acl), widgets);
    /* Apply button, saves all acl settings */
    GtkWidget *apply_acl_button = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_box_pack_start(GTK_BOX(acl_button_box), apply_acl_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(apply_acl_button), "clicked", 
                             G_CALLBACK(apply_server_settings), widgets);
    /* ACL TREEVIEW END */


    /* ACL Permissions TREEVIEW BEGIN */
    aclperm_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_box_pack_start(GTK_BOX(hbox[2]), aclperm_scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(aclperm_scrolled_window), 
						    GTK_POLICY_AUTOMATIC, 
						    GTK_POLICY_ALWAYS);
    /* Must set a larger size or it wont scroll */
    gtk_widget_set_size_request(aclperm_scrolled_window, -1, 100);

    widgets->aclperm_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    widgets->aclperm_treeview = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(widgets->aclperm_treeview), GTK_TREE_MODEL(widgets->aclperm_store));
    gtk_container_add(GTK_CONTAINER(aclperm_scrolled_window), widgets->aclperm_treeview);
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(widgets->aclperm_treeview), TRUE);

    /* Set the column labels in the treeview */
    GtkTreeViewColumn *aclpermname_col = gtk_tree_view_column_new_with_attributes(_("ACL Type"), combo_cell[z], "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->aclperm_treeview), GTK_TREE_VIEW_COLUMN(aclpermname_col));
    z++;
    GtkTreeViewColumn *aclpermtype_col = gtk_tree_view_column_new_with_attributes(_("ACL Permission"), combo_cell[z], "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->aclperm_treeview), GTK_TREE_VIEW_COLUMN(aclpermtype_col));
    z++;
    GtkTreeViewColumn *aclaccess_col = gtk_tree_view_column_new_with_attributes(_("ACL Name"), combo_cell[z], "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(widgets->aclperm_treeview), GTK_TREE_VIEW_COLUMN(aclaccess_col));
    z++;

    /* AddType/Application type button box */
    GtkWidget *aclperm_button_box = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(hbox[3]), aclperm_button_box, FALSE, TRUE, 0);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(aclperm_button_box), GTK_BUTTONBOX_SPREAD);

    /* Delete button */
    GtkWidget *delete_aclperm_button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
    gtk_box_pack_start(GTK_BOX(aclperm_button_box), delete_aclperm_button, FALSE, FALSE, 0);
    g_signal_connect(delete_aclperm_button, "clicked",  
	      G_CALLBACK(delete_row_clicked), widgets->aclperm_treeview);

    /* Add button */
    GtkWidget *add_aclperm_button = gtk_button_new_from_stock(GTK_STOCK_ADD);
    gtk_box_pack_start(GTK_BOX(aclperm_button_box), add_aclperm_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(add_aclperm_button), "clicked", 
                             G_CALLBACK(set_acl_type), "second_acl_treeview");
    g_signal_connect_swapped(G_OBJECT(add_aclperm_button), "clicked", 
                             G_CALLBACK(add_acl), widgets);

    /* Apply button, saves all acl settings */
    GtkWidget *apply_aclperm_button = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_box_pack_start(GTK_BOX(aclperm_button_box), apply_aclperm_button, FALSE, FALSE, 0);
    g_signal_connect_swapped(G_OBJECT(apply_aclperm_button), "clicked", 
                             G_CALLBACK(apply_server_settings), widgets);
    /* ACL Permissions TREEVIEW END */











    /* Connect text_cells renderers to the edited callback */
    for(i=0; i<NUM_TEXT_CELLS; i++)
    {
	g_signal_connect(text_cell[i], "edited",  
	    G_CALLBACK(text_cell_edited), widgets->acl_store);
    }

    /* Setup combo_cells data and callbacks */
    for(z=0; z<NUM_COMBO_CELLS; z++)
    {
	g_object_set(G_OBJECT(combo_cell[z]), "editable", TRUE, NULL);
	g_object_set(G_OBJECT(combo_cell[z]), "text-column", 0, NULL);

	/* The first and second aclperm combos
	   should also be manually editable */
	if( z==1 || z==3 )
	  g_object_set(combo_cell[z], "has-entry", TRUE, NULL);
	else
	  g_object_set(combo_cell[z], "has-entry", FALSE, NULL);

	/* Using different callback stores for different combo_cells */
	if( z == 0 )
	{
	    g_signal_connect(G_OBJECT(combo_cell[z]), "edited",
		G_CALLBACK(combo_cell_edited), widgets->acl_store);
	}
	else
	{
	    g_signal_connect(G_OBJECT(combo_cell[z]), "edited",
		G_CALLBACK(combo_cell_edited), widgets->aclperm_store);
	}
    }

    /* Populate the combo lists... */
    /* Create popdown strings for the first acl_treeview's Type combo */
    acl_combo_list[0] = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "src", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "dst", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "srcdomain", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "dstdomain", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "port", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "proto", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "method", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "time", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "urlpath_regex", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "srcdom_regex", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "dstdom_regex", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "rep_header", -1);
    gtk_list_store_append(acl_combo_list[0], &iter);
    gtk_list_store_set(acl_combo_list[0], &iter, 0, "rep_mime_type", -1);

    /* Create popdown strings for aclperms_treeviews ACL Type combo */
    acl_combo_list[1] = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "http_access", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "http_reply_access", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "icp_access", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "ident_lookup_access", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "snmp_access", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "cache", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "no_cache", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "always_direct", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "never_direct", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "broken_posts", -1);
    gtk_list_store_append(acl_combo_list[1], &iter);
    gtk_list_store_set(acl_combo_list[1], &iter, 0, "miss_access", -1);

    /* Create popdown strings for aclperms_treeviews ACL Permission combo */
    acl_combo_list[2] = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(acl_combo_list[2], &iter);
    gtk_list_store_set(acl_combo_list[2], &iter, 0, "Allow", -1);
    gtk_list_store_append(acl_combo_list[2], &iter);
    gtk_list_store_set(acl_combo_list[2], &iter, 0, "Deny", -1);

    /* Create an empty list for aclperms_treeviews ACL Name combo (editable) */
    acl_combo_list[3] = gtk_list_store_new(1, G_TYPE_STRING);


    /* Add combo list 0 to the first acl combo */
    g_object_set(G_OBJECT(combo_cell[0]), "model", 
    acl_combo_list[0], "editable", TRUE, NULL);

    /* Add combo list 1 to the first aclperm name combo.
       This list is populated with the contents of the
       first acl treeview's names whenever the first list
       is edited and at app startup in populate_acl_tab.c */
    g_object_set(G_OBJECT(combo_cell[1]), "model", 
    acl_combo_list[1], "editable", TRUE, NULL);

    /* Add combo list 2 to the second aclperms combo */
    g_object_set(G_OBJECT(combo_cell[2]), "model", 
    acl_combo_list[2], "editable", TRUE, NULL);

    /* Add combo list 3 to the third aclperms combo */
    g_object_set(G_OBJECT(combo_cell[3]), "model", 
    acl_combo_list[3], "editable", TRUE, NULL);

    /* Add column numbers to the cells */
    g_object_set_data(G_OBJECT(combo_cell[0]), "column", (void*)(1));

    /* Second treeview's cells */
    g_object_set_data(G_OBJECT(combo_cell[1]), "column", (void*)(0));
    g_object_set_data(G_OBJECT(combo_cell[2]), "column", (void*)(1));
    g_object_set_data(G_OBJECT(combo_cell[3]), "column", (void*)(2));

    gtk_widget_show_all(widgets->main_window);

    if( utf8 != NULL )
      g_free(utf8);
}
