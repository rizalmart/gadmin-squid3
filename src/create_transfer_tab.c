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
#include "create_transfer_tab.h"


void create_transfer_tab(struct w *widgets)
{


    return; // Use for delay pools and misc options





    /* Create the transfer textview in a scrolled window */
    GtkWidget *transfer_hbox[4], *transfer_scrolled_window;

    GtkWidget *top_spacer_label = gtk_label_new("");

    GtkWidget *right_spacer_label[3];
    right_spacer_label[0] = gtk_label_new("   ");
    right_spacer_label[1] = gtk_label_new("   ");
    right_spacer_label[2] = gtk_label_new("   ");

    GtkWidget *total_bandwidth_label = gtk_label_new(" Total transfer rate: ");
    GtkWidget *total_incoming_label = gtk_label_new(" Total incoming: ");
    GtkWidget *total_outgoing_label = gtk_label_new(" Total outgoing: ");
    widgets->total_bandwidth_entry = gtk_entry_new();
    widgets->total_incoming_entry = gtk_entry_new();
    widgets->total_outgoing_entry = gtk_entry_new();
    widgets->kick_entry = gtk_entry_new();

    GtkWidget *kick_descr_label = gtk_label_new(" Username or Process ID: ");
    GtkWidget *kick_label, *kickban_label;
    GtkWidget *kick_button, *kickban_button;
    GtkWidget *kick_hbox, *kickban_hbox;
    GtkWidget *kick_image, *kickban_image;
    GtkWidget *kick_alignment, *kickban_alignment;

    /* A top spacer */
    transfer_hbox[0] = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox4), transfer_hbox[0], FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(transfer_hbox[0]), top_spacer_label, FALSE, FALSE, 0);


    /* Total transfer rate entry */
    transfer_hbox[1] = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox4), transfer_hbox[1], FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(transfer_hbox[1]), total_bandwidth_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(transfer_hbox[1]), widgets->total_bandwidth_entry, TRUE, TRUE, 0);
    gtk_widget_set_sensitive(widgets->total_bandwidth_entry, FALSE);    

    gtk_box_pack_start(GTK_BOX(transfer_hbox[1]), right_spacer_label[0], TRUE, TRUE, 0);


    /* Total incoming / total outgoing transfer rate entries */
    transfer_hbox[2] = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox4), transfer_hbox[2], FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(transfer_hbox[2]), total_incoming_label, FALSE, FALSE, 0);
    gtk_misc_set_alignment(GTK_MISC(total_incoming_label), 0, 0.5);

    gtk_box_pack_start(GTK_BOX(transfer_hbox[2]), widgets->total_incoming_entry, TRUE, TRUE, 0);
    gtk_widget_set_sensitive(widgets->total_incoming_entry, FALSE);    

    gtk_box_pack_start(GTK_BOX(transfer_hbox[2]), total_outgoing_label, FALSE, FALSE, 0);
    gtk_misc_set_alignment(GTK_MISC(total_outgoing_label), 0, 0.5);

    gtk_box_pack_start(GTK_BOX(transfer_hbox[2]), widgets->total_outgoing_entry, TRUE, TRUE, 0);
    gtk_widget_set_sensitive(widgets->total_outgoing_entry, FALSE);

    gtk_box_pack_start(GTK_BOX(transfer_hbox[2]), right_spacer_label[1], TRUE, TRUE, 0);



    /* Kick/kickban buttons and entry */
    transfer_hbox[3] = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox4), transfer_hbox[3], FALSE, TRUE, 0);
    
    gtk_box_pack_start(GTK_BOX(transfer_hbox[3]), kick_descr_label, FALSE, FALSE, 0);
    gtk_misc_set_alignment(GTK_MISC(kick_descr_label), 0, 0.5);

    widgets->kick_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(transfer_hbox[3]), widgets->kick_entry, TRUE, TRUE, 0);

    kick_button = gtk_button_new();
    kick_alignment = gtk_alignment_new (0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(kick_button), kick_alignment);

    kick_hbox = gtk_hbox_new(FALSE, 2);
    gtk_container_add(GTK_CONTAINER(kick_alignment), kick_hbox);

    kick_image = gtk_image_new_from_stock("gtk-apply", GTK_ICON_SIZE_BUTTON);
    gtk_box_pack_start(GTK_BOX(kick_hbox), kick_image, FALSE, FALSE, 0);

    kick_label = gtk_label_new_with_mnemonic(_("Kick"));
    gtk_box_pack_start(GTK_BOX(kick_hbox), kick_label, FALSE, FALSE, 0);

    gtk_label_set_justify(GTK_LABEL(kick_label), GTK_JUSTIFY_LEFT);

    gtk_box_pack_start(GTK_BOX(transfer_hbox[3]), kick_button, FALSE, FALSE, 0);
//    g_signal_connect_swapped(G_OBJECT(kick_button), "clicked",
//			     G_CALLBACK(kick_button_clicked), widgets);


    kickban_button = gtk_button_new();
    kickban_alignment = gtk_alignment_new (0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(kickban_button), kickban_alignment);

    kickban_hbox = gtk_hbox_new(FALSE, 2);
    gtk_container_add(GTK_CONTAINER(kickban_alignment), kickban_hbox);

    kickban_image = gtk_image_new_from_stock("gtk-delete", GTK_ICON_SIZE_BUTTON);
    gtk_box_pack_start(GTK_BOX(kickban_hbox), kickban_image, FALSE, FALSE, 0);

    kickban_label = gtk_label_new_with_mnemonic(_("Kickban"));
    gtk_box_pack_start(GTK_BOX(kickban_hbox), kickban_label, FALSE, FALSE, 0);

    gtk_label_set_justify(GTK_LABEL(kickban_label), GTK_JUSTIFY_LEFT);

    gtk_box_pack_start(GTK_BOX(transfer_hbox[3]), kickban_button, FALSE, FALSE, 0);

//    g_signal_connect_swapped(G_OBJECT(kickban_button), "clicked",
//			     G_CALLBACK(kick_ban_button_clicked), widgets);


    gtk_box_pack_start(GTK_BOX(transfer_hbox[3]), right_spacer_label[2], TRUE, TRUE, 0);


    /* A scrolled window with the transfer textview */
    transfer_hbox[4] = gtk_hbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widgets->notebook_vbox4), transfer_hbox[4], TRUE, TRUE, 0);

    transfer_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(transfer_hbox[4]), transfer_scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(transfer_scrolled_window), 
							    GTK_POLICY_AUTOMATIC, 
							    GTK_POLICY_ALWAYS);
    /* Must set a larger size or it wont scroll */
    gtk_widget_set_size_request(transfer_scrolled_window, -1, 140);

    widgets->transfer_textview = gtk_text_view_new();
    gtk_container_add (GTK_CONTAINER (transfer_scrolled_window), widgets->transfer_textview);


    gtk_widget_show_all(widgets->main_window);
}
