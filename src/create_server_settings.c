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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gettext.h"
#include "widgets.h"
#include "make_settings_entries.h"
#include "make_settings_combos.h"
#include "make_settings_spinbuttons.h"
#include "make_settings_progressbars.h"
#include "make_settings_labels.h"
#include "make_settings_hseparators.h"
#include "generate_cert.h"
#include "create_server_settings.h"

extern int use_tls;



void create_server_settings(struct w *widgets)
{
    GtkTooltip *tooltips;
//    GtkWidget *gen_cert_button;
    gchar *utf8 = NULL;
//    gdouble progress_val = 1.0;
    gchar *combo_text;

    /* Counters for the widgets and positions in the table */
    int a = 0;
    int b = 1;
    int ent  = 0;
    int spin = 0;
    int comb = 0;

    /* Max lengths and input */
    int entry_size = 100;
//    int combo_size = 100;
    //tooltips = gtk_tooltips_new();


    /* visible_hostname */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Visible hostname: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("A visible hostname, IE: www.example.org"));
    a++; b++; ent++;

    /* mail_from */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" The administrators email address: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("The administrators email address"));
    a++; b++; ent++;

    /* client_netmask */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Client netmask: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("The clients netmask IE: 255.255.255.0"));
    a++; b++; ent++;

    /* check_hostnames */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Check hostnames: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* icp_query_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" ICP query timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* maximum_icp_query_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Maximum ICP query timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* mcast_icp_query_timeout */    
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Multicast ICP query timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* dead_peer_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Dead peer timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* forward_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Forward timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of minutes"));
    a++; b++; spin++;

    /* connect_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Connect timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of minutes"));
    a++; b++; spin++;

    /* peer_connect_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Peer connect timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* read_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Read timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of minutes"));
    a++; b++; spin++;

    /* request_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Request timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of minutes"));
    a++; b++; spin++;

    /* persistent_request_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Persistent request timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of minutes"));
    a++; b++; spin++;

    /* pconn_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Persistent connection timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* ident_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Ident timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* dns_timeout */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" DNS timeout: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of minutes"));
    a++; b++; spin++;

    /* dns_retransmit_interval */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" DNS retransmit interval: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of seconds"));
    a++; b++; spin++;

    /* dns_defnames */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" DNS defnames: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* dns_nameservers */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" DNS name servers: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent],
    _("IE: 10.0.0.1 , 192.168.0.100 or clear this field to use system resolvers from /etc/resolv.conf"));
    a++; b++; ent++;

    /* snmp_port */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" SNMP port: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("The SNMP port to use"));
    a++; b++; spin++;

    /* snmp_incoming_address */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" SNMP incoming address: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("IE: 0.0.0.0"));
    a++; b++; ent++;

    /* snmp_outgoing_address */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" SNMP outgoing address: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("IE: 0.0.0.0"));
    a++; b++; ent++;

    /* udp_incoming_address */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" UDP incoming address: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("UDP incoming address"));
    a++; b++; ent++;

    /* udp_outgoing_address */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" UDP outgoing address: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("UDP outgoing address"));
    a++; b++; ent++;

    /* icp_port (icp_port 3130 proxy-only) */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" ICP port: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("The ICP port"));
    a++; b++; ent++;

    /* cache_mem */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache memory: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 5 MB"));
    a++; b++; spin++;

    /* cache_swap_low */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache swap low: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 50"));
    a++; b++; spin++;

    /* cache_swap_high */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache swap high: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 500000"));
    a++; b++; spin++;

    /* maximum_object_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Maximum object size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 4000"));
    a++; b++; spin++;

    /* minimum_object_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Mimimum object size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 4"));
    a++; b++; spin++;

    /* maximum_object_size_in_memory */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Maximum object size in memory: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 4244"));
    a++; b++; spin++;

    /* ipcache_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" IPcache size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 5576"));
    a++; b++; spin++;

    /* ipcache_low */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" IPcache low: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 42"));
    a++; b++; spin++;

    /* ipcache_high */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" IPcache high: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 442"));
    a++; b++; spin++;

    /* fqdncache_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" FQDN cache size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 456"));
    a++; b++; spin++;

    /* cache_replacement_policy */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache replacement policy: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("IE: lru"));
    a++; b++; ent++;

    /* memory_replacement_policy */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Memory replacement policy: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("IE: lru"));
    a++; b++; ent++;

    /* cache_dir */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache dir: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Path to the squid cache directory"));
    a++; b++; ent++;

    /* hierarchy_stoplist */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Hierarchy stoplist: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("IE: A stoplist"));
    a++; b++; ent++;

    /* access_log */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Access log: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Path to the squid access log"));
    a++; b++; ent++;

    /* cache_log */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache log: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Path to squids cache log"));
    a++; b++; ent++;

    /* cache_store_log */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache store log: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Path to squids store log"));
    a++; b++; ent++;

    /* mime_table (Better off not using this option) */
//    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
//    _(" Mime table: "), 0,1,a,b,entry_size);
//    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("IE: hmm"), NULL);
//    a++; b++; ent++;

    /* pid_filename */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" PID file: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Path to squids PID file"));
    a++; b++; ent++;

    /* hosts_file */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Hosts file: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("IE: /etc/hosts"));
    a++; b++; ent++;

    /* icon_directory */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Icon directory: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Icon directory path"));
    a++; b++; ent++;

    /* error_directory */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Error directory: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Error directory path"));
    a++; b++; ent++;

    /* diskd_program */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Diskd program: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Path to the diskd program"));
    a++; b++; ent++;

    /* unlinkd_program */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Unlinkd program: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Path to the unlinkd program"));
    a++; b++; ent++;

    /* debug_options */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Debug options: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Debug options"));
    a++; b++; ent++;

    /* ftp_user */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" FTP user: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("The FTP user name"));
    a++; b++; ent++;

    /* emulate_httpd_log */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Emulate HTTPD log: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* log_ip_on_direct */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Log ip on direct: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* log_mime_hdrs */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Log mime headers: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* log_fqdn */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Log FQDN: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* ftp_passive */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" FTP passive: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* ftp_sanitycheck */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" FTP sanitycheck: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* ftp_telnet_protocol */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" FTP telnet protocol: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* allow_underscore */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Allow underscores: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* ftp_list_width */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" FTP list width: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("List width for FTP"));
    a++; b++; spin++;

    /* memory_pools */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Memory pools: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* memory_pools_limit 5 MB */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Memory pool limit: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IE: 5 MB"));
    a++; b++; spin++;

    /* half_closed_clients */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Half closed clients: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* httpd_suppress_version_string */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" HTTPD suppress version string: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* via */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Proxy via: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* forwarded_for */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Forwarded for: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* log_icp_queries */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Log ICP queries: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* client_db */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Client database: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* icp_hit_stale */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" ICP hit stale: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* query_icmp */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Query ICMP: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* test_reachability */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Test reachability: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* buffered_logs */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Buffered logs: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* reload_into_ims */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Reload into IMS: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* global_internal_static */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Global internal static: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* short_icon_urls */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Short icon URLs: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* offline_mode */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Offline mode: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* uri_whitespace */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" URL whitespace: "), 0,1,a,b,entry_size);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("No information found"));
    a++; b++; ent++;

    /* nonhierarchical_direct */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Nonhierarchical direct: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* prefer_direct */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Prefer direct: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* strip_query_terms */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Strip query terms: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* redirector_bypass */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Redirector bypass: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* ignore_unknown_nameservers */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Ignore unknown nameservers: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* client_persistent_connections */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Client use persistent connections: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* server_persistent_connections */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Server use persistent connections: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* persistent_connection_after_error */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Persistent connection after error: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* detect_broken_pconn */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Detect broken persistent connections: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* balance_on_multiple_ip */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Balance on multiple IPs: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* pipeline_prefetch */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Pipeline prefetch: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* request_entities */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Request entities: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* ie_refresh */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" IE refresh: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* vary_ignore_expire */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Vary ignore expire: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* relaxed_header_parser */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Relaxed header parser: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* retry_on_error */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Retry on error: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* Server user */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" The server runs as this user: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("The server drops root privileges and runs as this user"));
    a++; b++; ent++;

    /* Server group */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" The server runs as this group: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("The server drops root privileges and runs as this group"));
    a++; b++; ent++;

    /* cache_mgr */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Cache manager: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("The cache manager"));
    a++; b++; ent++;

    /* mail_program */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Mail program: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Mail program to use"));
    a++; b++; ent++;

    /* umask */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Umask: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Umask to use"));
    a++; b++; ent++;

    /* request_header_max_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Request header max size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Limit request headers"));
    a++; b++; spin++;

    /* request_body_max_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Request body max size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Limit body sizes"));
    a++; b++; spin++;

    /* quick_abort_min */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Quick abort minimum: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Quick abort on minimum"));
    a++; b++; spin++;

    /* quick_abort_max */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Quick abort maximum: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Quick abort on maximum"));
    a++; b++; spin++;

    /* quick_abort_pct */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Quick abort pct: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("PCT abort"));
    a++; b++; spin++;

    /* read_ahead_gap */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Read ahead gap: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Seconds"));
    a++; b++; spin++;

    /* negative_ttl */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Negative ttl: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Minutes"));
    a++; b++; spin++;

    /* positive_dns_ttl */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Positive DNS TTL: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("DNS time to live"));
    a++; b++; spin++;

    /* negative_dns_ttl */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Negative DNS TTL: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Negative DNS time to live"));
    a++; b++; spin++;

    /* range_offset_limit */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Range offset limit: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("A range offset"));
    a++; b++; spin++;

    /* client_lifetime */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Client lifetime: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Number of days"));
    a++; b++; spin++;

    /* shutdown_lifetime */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Shutdown lifetime: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("A shutdown lifetime"));
    a++; b++; spin++;

    /* reply_header_max_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Reply header max size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Max reply header"));
    a++; b++; spin++;

    /* announce_period */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Announce_period: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Announcement interval"));
    a++; b++; spin++;

    /* announce_port */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Announce_port: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Port to announce to"));
    a++; b++; spin++;

    /* announce_host */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Announce host: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Host to announce to"));
    a++; b++; ent++;

    /* logfile_rotate */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Logfile rotate: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Logfile rotation"));
    a++; b++; spin++;

    /* tcp_recv_bufsize */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" TCP recieve buffer size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("TCP recive buffer size"));
    a++; b++; spin++;

    /* minimum_direct_hops */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Minimum direct hops: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Minimum direct hops"));
    a++; b++; spin++;

    /* minimum_direct_rtt */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Minimum direct rtt: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Minimum direct RTT"));
    a++; b++; spin++;

    /* store_avg_object_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Store average object size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Storage average object size"));
    a++; b++; spin++;

    /* store_objects_per_bucket */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Store objects per bucket: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Objects per bucket"));
    a++; b++; spin++;

    /* netdb_low */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Net database low: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Net DB low"));
    a++; b++; spin++;

    /* netdb_high */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Net database high: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Net DB high"));
    a++; b++; spin++;

    /* netdb_ping_period */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Net database ping period: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Minutes"));
    a++; b++; spin++;

    /* maximum_single_addr_tries */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Maximum single address tries: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Max single address tries"));
    a++; b++; spin++;

    /* as_whois_server */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Use this whois server: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("A whois server to use"));
    a++; b++; ent++;

    /* wccp_address */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP address: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Address"));
    a++; b++; ent++;

    /* wccp2_address */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP2 address: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Address"));
    a++; b++; ent++;

    /* wccp_router */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP router: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Router"));
    a++; b++; ent++;

    /* wccp_version */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP version: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Version"));
    a++; b++; spin++;

    /* wccp2_rebuild_wait */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP2 rebuild wait: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* wccp2_forwarding_method */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP2 forwarding method: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Forwarding method"));
    a++; b++; spin++;

    /* wccp2_return_method */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP2 return method: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Return method"));
    a++; b++; spin++;

    /* wccp2_assignment_method */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP2 assignment method: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Assignment method"));
    a++; b++; spin++;

    /* wccp2_service (standard) */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP2 service: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Service"));
    a++; b++; spin++;

    /* wccp2_weight */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" WCCP2 weight: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Weight"));
    a++; b++; spin++;

    /* incoming_icp_average */
/*    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Incoming ICP average: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("No information found"), NULL);
    a++; b++; spin++;
*/
    spin++;

    /* incoming_http_average */
/*    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Incoming HTTP average: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("No information found"), NULL);
    a++; b++; spin++;
*/
    spin++;

    /* incoming_dns_average */
/*    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Incoming DNS average: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("No information found"), NULL);
    a++; b++; spin++;
*/
    spin++;

    /* min_icp_poll_cnt */
/*    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Minimum ICP poll count: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("No information found"), NULL);
    a++; b++; spin++;
*/
    spin++;

    /* min_dns_poll_cnt */
/*    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Minimum DNS poll count: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("No information found"), NULL);
    a++; b++; spin++;
*/
    spin++;

    /* min_http_poll_cnt */
/*    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Minimum HTTP poll count: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("No information found"), NULL);
    a++; b++; spin++;
*/
    spin++;

    /* max_open_disk_fds */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Maximum open disk fds: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Max open disk fd's"));
    a++; b++; spin++;

    /* digest_generation */
    widgets->server_set_combo[comb] = make_short_combo_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Digest generation: "), 0,1,a,b,50);
    combo_text = g_strdup_printf(_("On"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    combo_text = g_strdup_printf(_("Off"));
    utf8 = g_locale_to_utf8(combo_text, strlen(combo_text), NULL, NULL, NULL);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX(widgets->server_set_combo[comb]), utf8);
    g_free(combo_text); g_free(utf8);
    a++; b++; comb++;

    /* digest_bits_per_entry */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Digest bits per entry: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Digest bits"));
    a++; b++; spin++;

    /* digest_rebuild_period */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Digest rebuild period: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Rebuild period"));
    a++; b++; spin++;

    /* digest_rewrite_period */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Digest rewrite period: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Rewrite period"));
    a++; b++; spin++;

    /* digest_swapout_chunk_size */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Digest swapout chunk size: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Chunk size"));
    a++; b++; spin++;

    /* digest_rebuild_chunk_percentage */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Digest rebuild chunk percentage: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Chunk percentage"));
    a++; b++; spin++;

    /* high_response_time_warning */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" High response time warning: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Warn on high response times"));
    a++; b++; spin++;

    /* high_page_fault_warning */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" High page fault warning: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Warn on high page faults"));
    a++; b++; spin++;

    /* high_memory_warning */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" High memory warning: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Warn if memory usage is higher then this limit in MegaBytes"));
    a++; b++; spin++;

    /* sleep_after_fork */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Sleep after fork: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Sleep after forking"));
    a++; b++; spin++;

    /* Use COMBO ??? store_dir_select_algorithm least-load... */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Store directory selection algorithm: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Algorithm"));
    a++; b++; ent++;

    /* minimum_expiry_time */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Minimum expiry time: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Min expiry time"));
    a++; b++; spin++;

    /* authenticate_cache_garbage_interval */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Authenticate cache garbage interval: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("Interval"));
    a++; b++; spin++;

    /* authenticate_ttl */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Authenticate TTL: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("TTL"));
    a++; b++; spin++;

    /* authenticate_ip_ttl */
    widgets->server_set_spinbutton[spin] = make_short_spinbutton_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Authenticate IP TTL: "), 0,1,a,b,50);
    gtk_widget_set_tooltip_text(widgets->server_set_spinbutton[spin], _("IP TTL"));
    a++; b++; spin++;

    /* coredump_dir */
    widgets->server_set_entry[ent] = make_long_entry_with_label(GTK_TABLE(widgets->srv_set_table),
    _(" Coredump directory: "), 0,1,a,b,200);
    gtk_widget_set_tooltip_text(widgets->server_set_entry[ent], _("Directory for coredumps"));
    a++; b++; ent++;

    gtk_widget_show_all(widgets->main_window);
}
