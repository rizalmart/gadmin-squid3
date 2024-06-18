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
#include <stdlib.h>
#include <string.h>
#include "widgets.h"
#include "gettext.h"
#include "reread_conf.h"
#include "show_info.h"
#include "functions.h"
#include "apply_server_settings.h"
#include "allocate.h"
#include "commented.h"
#include "get_option_pos.h"
#include "populate_servers.h"
#include "populate_server_settings.h"
#include "select_first_server.h"
#include "populate_acl_tab.h"
#include "populate_conf_tab.h"


extern char global_server_address[1024];
extern char global_server_port[1024];
extern char global_server_name[1024];
extern char global_server_type[1024];

extern int use_tls;
extern int activated;

/* ACL type to add to modules_buffer (0=definition, 1=limiting/implementation) */
int acl_type = 0;

/* ACL permission to add (allow or deny) */
int acl_perm = 0;

/* Maximum size of all server lines */
#define SERVER_BUFFER_MAX_SIZE 20000
char *server_buffer;

/* Maximum size of all ACL lines */
#define ACL_BUFFER_MAX_SIZE 600000
char *acl_buffer;

/* Maximum size of all option lines */
#define OPTION_BUFFER_MAX_SIZE 600000



int add_servers_to_buffer(GtkTreeModel *model, GtkTreePath *path,
        	          GtkTreeIter *iter, struct w *widgets)
{
    gchar *conf_line=NULL;
    gchar *option1=NULL, *option2=NULL, *option3=NULL;

    gtk_tree_model_get(model, iter, 0, &option1, -1);
    gtk_tree_model_get(model, iter, 1, &option2, -1);
    gtk_tree_model_get(model, iter, 2, &option3, -1);

    /* The first 2 columns must be filled in. */
    if( option1==NULL || option2==NULL )
    {
	printf("Missing value for a server.\n");
	if( option1!=NULL )
          g_free(option1);
	if( option2!=NULL )
          g_free(option2);
	if( option3!=NULL )
          g_free(option3);

    	return FALSE;
    }

    /* http_port, https_port etc based on if the third column is filled in */
    if( option3==NULL )
      conf_line = g_strdup_printf("http_port %s:%s\n", option1, option2);
    else
      conf_line = g_strdup_printf("https_port %s:%s %s\n", option1, option2, option3);

    /* Add to global buffer */
    strcat(server_buffer, conf_line);

    if( conf_line!=NULL )
      g_free(conf_line);

    if( option1!=NULL )
      g_free(option1);
    if( option2!=NULL )
      g_free(option2);
    if( option3!=NULL )
      g_free(option3);

    /* Return false to continue iteration */
    return FALSE; 
}


int add_acls_to_buffer(GtkTreeModel *model, GtkTreePath *path,
        	          GtkTreeIter *iter, struct w *widgets)
{
    gchar *conf_line=NULL;
    gchar *option1, *option2, *option3;

    gtk_tree_model_get(model, iter, 0, &option1, -1);
    gtk_tree_model_get(model, iter, 1, &option2, -1);
    gtk_tree_model_get(model, iter, 2, &option3, -1);

    /* The input must be complete. */
    if( option1==NULL
    ||  option2==NULL
    ||  option3==NULL )
    {
	printf("Missing value for an acl tab option.\n");
	if( option1!=NULL )
          g_free(option1);
	if( option2!=NULL )
          g_free(option2);
	if( option3!=NULL )
          g_free(option3);

    	return FALSE;
    }

    /* ACL definitions IE: "acl our_networks src something" */
    if( acl_type == 0 )
    {
        conf_line = g_strdup_printf("acl %s %s %s\n", option1, option2, option3);
	/* Add to global buffer */
	strcat(acl_buffer, conf_line);
    }

    /* ACL allow settings IE: "http_access allnets allow" */
    if( acl_type == 1 && cmplowercase(option2, "allow") && acl_perm == 0 )
    {
        conf_line = g_strdup_printf("%s allow %s\n", option1, option3);
	strcat(acl_buffer, conf_line);
    }

    /* ACL deny settings IE: "http_access allnets deny" */
    if( acl_type == 1 && cmplowercase(option2, "deny")  && acl_perm == 1 )
    {
        conf_line = g_strdup_printf("%s deny %s\n", option1, option3);
	strcat(acl_buffer, conf_line);
    }

    if( conf_line!=NULL )
      g_free(conf_line);

    if( option1!=NULL )
      g_free(option1);
    if( option2!=NULL )
      g_free(option2);
    if( option3!=NULL )
      g_free(option3);

    /* Return false to continue iteration */
    return FALSE; 
}


void apply_server_settings(struct w *widgets)
{
    /* Change and save all server settings. */
    FILE *fp;
    gint active_index;
    char *config;
    GtkTreeModel *model;
    int i = 0;

    G_CONST_RETURN gchar *value;
    char *opt_buffer, *opt_line;
    opt_buffer = allocate(300000);
    opt_line   = allocate(1024);

    /* Add entry settings to options buffer */
    for(i=0; i<=36; i++)
    {
        value = g_strdup_printf("%s", gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[i])));
        if( value==NULL || strlen(value) < 1 )
	{
//	    printf("Entry %d skipped. Input too short\n", i);
	    continue;
	}

	if( i == 0 )
	  snprintf(opt_line, 1000, "visible_hostname %s\n", value);
    	if( i == 1 )
	  snprintf(opt_line, 1000, "mail_from %s\n", value);
    	if( i == 2 )
	  snprintf(opt_line, 1000, "client_netmask %s\n", value);
    	if( i == 3 )
	  snprintf(opt_line, 1000, "dns_nameservers %s\n", value);
    	if( i == 4 )
	  snprintf(opt_line, 1000, "snmp_incoming_address %s\n", value);
    	if( i == 5 )
	  snprintf(opt_line, 1000, "snmp_outgoing_address %s\n", value);
    	if( i == 6 )
	  snprintf(opt_line, 1000, "udp_incoming_address %s\n", value);
    	if( i == 7 )
	  snprintf(opt_line, 1000, "udp_outgoing_address %s\n", value);
    	if( i == 8 )
	  snprintf(opt_line, 1000, "icp_port %s\n", value);
    	if( i == 9 )
	  snprintf(opt_line, 1000, "cache_replacement_policy %s\n", value);
    	if( i == 10 )
	  snprintf(opt_line, 1000, "memory_replacement_policy %s\n", value);
    	if( i == 11 )
	  snprintf(opt_line, 1000, "cache_dir %s\n", value);
    	if( i == 12 )
	  snprintf(opt_line, 1000, "hierarchy_stoplist %s\n", value);
    	if( i == 13 )
	  snprintf(opt_line, 1000, "access_log %s\n", value);
    	if( i == 14 )
	  snprintf(opt_line, 1000, "cache_log %s\n", value);
    	if( i == 15 )
	  snprintf(opt_line, 1000, "cache_store_log %s\n", value);
    	if( i == 16 )
	  snprintf(opt_line, 1000, "pid_filename %s\n", value);
    	if( i == 17 )
	  snprintf(opt_line, 1000, "hosts_file %s\n", value);
    	if( i == 18 )
	  snprintf(opt_line, 1000, "icon_directory %s\n", value);
    	if( i == 19 )
	  snprintf(opt_line, 1000, "error_directory %s\n", value);
    	if( i == 20 )
	  snprintf(opt_line, 1000, "diskd_program %s\n", value);
    	if( i == 21 )
	  snprintf(opt_line, 1000, "unlinkd_program %s\n", value);
    	if( i == 22 )
	  snprintf(opt_line, 1000, "debug_options %s\n", value);
    	if( i == 23 )
	  snprintf(opt_line, 1000, "ftp_user %s\n", value);
    	if( i == 24 )
	  snprintf(opt_line, 1000, "uri_whitespace %s\n", value);
    	if( i == 25 )
	  snprintf(opt_line, 1000, "cache_effective_user %s\n", value);
    	if( i == 26 )
	  snprintf(opt_line, 1000, "cache_effective_group %s\n", value);
    	if( i == 27 )
	  snprintf(opt_line, 1000, "cache_mgr %s\n", value);
    	if( i == 28 )
	  snprintf(opt_line, 1000, "mail_program %s\n", value);
    	if( i == 29 )
	  snprintf(opt_line, 1000, "umask %s\n", value);
    	if( i == 30 )
	  snprintf(opt_line, 1000, "announce_host %s\n", value);
    	if( i == 31 )
	  snprintf(opt_line, 1000, "as_whois_server %s\n", value);
    	if( i == 32 )
	  snprintf(opt_line, 1000, "wccp_address %s\n", value);
    	if( i == 33 )
	  snprintf(opt_line, 1000, "wccp2_address %s\n", value);
    	if( i == 34 )
	  snprintf(opt_line, 1000, "wccp_router %s\n", value);
    	if( i == 35 )
	  snprintf(opt_line, 1000, "store_dir_select_algorithm %s\n", value);
    	if( i == 36 )
	  snprintf(opt_line, 1000, "coredump_dir %s\n", value);

	/* Add entry config values to the options buffer */
	strcat(opt_buffer, opt_line);
    }

    /* Add spinbutton settings to options buffer */
    for(i=0; i<=77; i++)
    {
	/* Skip some out commented options */
	if( i > 57 && i < 64 )
	  continue;

        value = g_strdup_printf("%s", gtk_entry_get_text(GTK_ENTRY(widgets->server_set_spinbutton[i])));
        if( value==NULL || strlen(value) < 1 )
	{
//	    printf("Spinbutton %d skipped. Input too short\n", i);
	    continue;
	}

	if( i == 0 )
	  snprintf(opt_line, 1000, "icp_query_timeout %s\n", value);
    	if( i == 1 )
	  snprintf(opt_line, 1000, "maximum_icp_query_timeout %s\n", value);
    	if( i == 2 )
	  snprintf(opt_line, 1000, "mcast_icp_query_timeout %s\n", value);
    	if( i == 3 )
	  snprintf(opt_line, 1000, "dead_peer_timeout %s seconds\n", value);
    	if( i == 4 )
	  snprintf(opt_line, 1000, "forward_timeout %s minutes\n", value);
    	if( i == 5 )
	  snprintf(opt_line, 1000, "connect_timeout %s minutes\n", value);
    	if( i == 6 )
	  snprintf(opt_line, 1000, "peer_connect_timeout %s seconds\n", value);
    	if( i == 7 )
	  snprintf(opt_line, 1000, "read_timeout %s minutes\n", value);
    	if( i == 8 )
	  snprintf(opt_line, 1000, "request_timeout %s minutes\n", value);
    	if( i == 9 )
	  snprintf(opt_line, 1000, "persistent_request_timeout %s minutes\n", value);
    	if( i == 10 )
	  snprintf(opt_line, 1000, "pconn_timeout %s seconds\n", value);
    	if( i == 11 )
	  snprintf(opt_line, 1000, "ident_timeout %s seconds\n", value);
    	if( i == 12 )
	  snprintf(opt_line, 1000, "dns_timeout %s minutes\n", value);
    	if( i == 13 )
	  snprintf(opt_line, 1000, "dns_retransmit_interval %s seconds\n", value);
    	if( i == 14 )
	  snprintf(opt_line, 1000, "snmp_port %s\n", value);
    	if( i == 15 )
	  snprintf(opt_line, 1000, "cache_mem %s MB\n", value);
    	if( i == 16 )
	  snprintf(opt_line, 1000, "cache_swap_low %s\n", value);
    	if( i == 17 )
	  snprintf(opt_line, 1000, "cache_swap_high %s\n", value);
    	if( i == 18 )
	  snprintf(opt_line, 1000, "maximum_object_size %s KB\n", value);
    	if( i == 19 )
	  snprintf(opt_line, 1000, "minimum_object_size %s KB\n", value);
    	if( i == 20 )
	  snprintf(opt_line, 1000, "maximum_object_size_in_memory %s KB\n", value);
    	if( i == 21 )
	  snprintf(opt_line, 1000, "ipcache_size %s\n", value);
    	if( i == 22 )
	  snprintf(opt_line, 1000, "ipcache_low %s\n", value);
    	if( i == 23 )
	  snprintf(opt_line, 1000, "ipcache_high %s\n", value);
    	if( i == 24 )
	  snprintf(opt_line, 1000, "fqdncache_size %s\n", value);
    	if( i == 25 )
	  snprintf(opt_line, 1000, "ftp_list_width %s\n", value);
    	if( i == 26 )
	  snprintf(opt_line, 1000, "memory_pools_limit %s MB\n", value);
    	if( i == 27 )
	  snprintf(opt_line, 1000, "request_header_max_size %s KB\n", value);
    	if( i == 28 )
	  snprintf(opt_line, 1000, "request_body_max_size %s KB\n", value);
    	if( i == 29 )
	  snprintf(opt_line, 1000, "quick_abort_min %s KB\n", value);
    	if( i == 30 )
	  snprintf(opt_line, 1000, "quick_abort_max %s KB\n", value);
    	if( i == 31 )
	  snprintf(opt_line, 1000, "quick_abort_pct %s\n", value);
    	if( i == 32 )
	  snprintf(opt_line, 1000, "read_ahead_gap %s KB\n", value);
    	if( i == 33 )
	  snprintf(opt_line, 1000, "negative_ttl %s minutes\n", value);
    	if( i == 34 )
	  snprintf(opt_line, 1000, "positive_dns_ttl %s seconds\n", value);
    	if( i == 35 )
	  snprintf(opt_line, 1000, "negative_dns_ttl %s seconds\n", value);
    	if( i == 36 )
	  snprintf(opt_line, 1000, "range_offset_limit %s KB\n", value);
    	if( i == 37 )
	  snprintf(opt_line, 1000, "client_lifetime %s day\n", value);
    	if( i == 38 )
	  snprintf(opt_line, 1000, "shutdown_lifetime %s seconds\n", value);
    	if( i == 39 )
	  snprintf(opt_line, 1000, "reply_header_max_size %s KB\n", value);
    	if( i == 40 )
	  snprintf(opt_line, 1000, "announce_period %s seconds\n", value);
    	if( i == 41 )
	  snprintf(opt_line, 1000, "announce_port %s\n", value);
    	if( i == 42 )
	  snprintf(opt_line, 1000, "logfile_rotate %s\n", value);
    	if( i == 43 )
	  snprintf(opt_line, 1000, "tcp_recv_bufsize %s bytes\n", value);
    	if( i == 44 )
	  snprintf(opt_line, 1000, "minimum_direct_hops %s\n", value);
    	if( i == 45 )
	  snprintf(opt_line, 1000, "minimum_direct_rtt %s\n", value);
    	if( i == 46 )
	  snprintf(opt_line, 1000, "store_avg_object_size %s KB\n", value);
    	if( i == 47 )
	  snprintf(opt_line, 1000, "store_objects_per_bucket %s\n", value);
    	if( i == 48 )
	  snprintf(opt_line, 1000, "netdb_low %s\n", value);
    	if( i == 49 )
	  snprintf(opt_line, 1000, "netdb_high %s\n", value);
    	if( i == 50 )
	  snprintf(opt_line, 1000, "netdb_ping_period %s minutes\n", value);
    	if( i == 51 )
	  snprintf(opt_line, 1000, "maximum_single_addr_tries %s\n", value);
    	if( i == 52 )
	  snprintf(opt_line, 1000, "wccp_version %s\n", value);
    	if( i == 53 )
	  snprintf(opt_line, 1000, "wccp2_forwarding_method %s\n", value);
    	if( i == 54 )
	  snprintf(opt_line, 1000, "wccp2_return_method %s\n", value);
    	if( i == 55 )
	  snprintf(opt_line, 1000, "wccp2_assignment_method %s\n", value);
    	if( i == 56 ) // Can also be "wccp2_service dynamic 80 password=hey".
	  snprintf(opt_line, 1000, "wccp2_service standard %s\n", value);
    	if( i == 57 )
	  snprintf(opt_line, 1000, "wccp2_weight %s\n", value);
/*
    	if( i == 58 )
	  snprintf(opt_line, 1000, "incoming_icp_average %s\n", value);
    	if( i == 59 )
	  snprintf(opt_line, 1000, "incoming_http_average %s\n", value);
    	if( i == 60 )
	  snprintf(opt_line, 1000, "incoming_dns_average %s\n", value);
    	if( i == 61 )
	  snprintf(opt_line, 1000, "min_icp_poll_cnt %s\n", value);
    	if( i == 62 )
	  snprintf(opt_line, 1000, "min_dns_poll_cnt %s\n", value);
    	if( i == 63 )
	  snprintf(opt_line, 1000, "min_http_poll_cnt %s\n", value);
*/
    	if( i == 64 )
	  snprintf(opt_line, 1000, "max_open_disk_fds %s\n", value);
    	if( i == 65 )
	  snprintf(opt_line, 1000, "digest_bits_per_entry %s\n", value);
    	if( i == 66 )
	  snprintf(opt_line, 1000, "digest_rebuild_period %s seconds\n", value);
    	if( i == 67 )
	  snprintf(opt_line, 1000, "digest_rewrite_period %s seconds\n", value);
    	if( i == 68 )
	  snprintf(opt_line, 1000, "digest_swapout_chunk_size %s bytes\n", value);
    	if( i == 69 )
	  snprintf(opt_line, 1000, "digest_rebuild_chunk_percentage %s\n", value);
    	if( i == 70 )
	  snprintf(opt_line, 1000, "high_response_time_warning %s\n", value);
    	if( i == 71 )
	  snprintf(opt_line, 1000, "high_page_fault_warning %s\n", value);
    	if( i == 72 )
	  snprintf(opt_line, 1000, "high_memory_warning %s MB\n", value);
    	if( i == 73 )
	  snprintf(opt_line, 1000, "sleep_after_fork %s\n", value);
    	if( i == 74 )
	  snprintf(opt_line, 1000, "minimum_expiry_time %s seconds\n", value);
    	if( i == 75 )
	  snprintf(opt_line, 1000, "authenticate_cache_garbage_interval %s seconds\n", value);
    	if( i == 76 )
	  snprintf(opt_line, 1000, "authenticate_ttl %s seconds\n", value);
    	if( i == 77 )
	  snprintf(opt_line, 1000, "authenticate_ip_ttl %s seconds\n", value);

	/* Add spinbutton config values to the options buffer */
	strcat(opt_buffer, opt_line);
    }


    /* Add combo box settings to options buffer */
    for(i=0; i<=42; i++)
    {
	active_index = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->server_set_combo[i]));
        if( active_index == 0 )
          value = g_strdup_printf("on");
	else
          value = g_strdup_printf("off");
	
        if( value==NULL || strlen(value) < 1 )
	{
//	    printf("Combo box %d skipped. Input too short\n", i);
	    continue;
	}

	if( i == 0 )
	  snprintf(opt_line, 1000, "check_hostnames %s\n", value);
	if( i == 1 )
	  snprintf(opt_line, 1000, "dns_defnames %s\n", value);
	if( i == 2 )
	  snprintf(opt_line, 1000, "emulate_httpd_log %s\n", value);
	if( i == 3 )
	  snprintf(opt_line, 1000, "log_ip_on_direct %s\n", value);
	if( i == 4 )
	  snprintf(opt_line, 1000, "log_mime_hdrs %s\n", value);
	if( i == 5 )
	  snprintf(opt_line, 1000, "log_fqdn %s\n", value);
	if( i == 6 )
	  snprintf(opt_line, 1000, "ftp_passive %s\n", value);
	if( i == 7 )
	  snprintf(opt_line, 1000, "ftp_sanitycheck %s\n", value);
	if( i == 8 )
	  snprintf(opt_line, 1000, "ftp_telnet_protocol %s\n", value);
	if( i == 9 )
	  snprintf(opt_line, 1000, "allow_underscore %s\n", value);
	if( i == 10 )
	  snprintf(opt_line, 1000, "memory_pools %s\n", value);
	if( i == 11 )
	  snprintf(opt_line, 1000, "half_closed_clients %s\n", value);
	if( i == 12 )
	  snprintf(opt_line, 1000, "httpd_suppress_version_string %s\n", value);
	if( i == 13 )
	  snprintf(opt_line, 1000, "via %s\n", value);
	if( i == 14 )
	  snprintf(opt_line, 1000, "forwarded_for %s\n", value);
	if( i == 15 )
	  snprintf(opt_line, 1000, "log_icp_queries %s\n", value);
	if( i == 16 )
	  snprintf(opt_line, 1000, "client_db %s\n", value);
	if( i == 17 )
	  snprintf(opt_line, 1000, "icp_hit_stale %s\n", value);
	if( i == 18 )
	  snprintf(opt_line, 1000, "query_icmp %s\n", value);
	if( i == 19 )
	  snprintf(opt_line, 1000, "test_reachability %s\n", value);
	if( i == 20 )
	  snprintf(opt_line, 1000, "buffered_logs %s\n", value);
	if( i == 21 )
	  snprintf(opt_line, 1000, "reload_into_ims %s\n", value);
	if( i == 22 )
	  snprintf(opt_line, 1000, "global_internal_static %s\n", value);
	if( i == 23 )
	  snprintf(opt_line, 1000, "short_icon_urls %s\n", value);
	if( i == 24 )
	  snprintf(opt_line, 1000, "offline_mode %s\n", value);
	if( i == 25 )
	  snprintf(opt_line, 1000, "nonhierarchical_direct %s\n", value);
	if( i == 26 )
	  snprintf(opt_line, 1000, "prefer_direct %s\n", value);
	if( i == 27 )
	  snprintf(opt_line, 1000, "strip_query_terms %s\n", value);
	if( i == 28 )
	  snprintf(opt_line, 1000, "redirector_bypass %s\n", value);
	if( i == 29 )
	  snprintf(opt_line, 1000, "ignore_unknown_nameservers %s\n", value);
	if( i == 30 )
	  snprintf(opt_line, 1000, "client_persistent_connections %s\n", value);
	if( i == 31 )
	  snprintf(opt_line, 1000, "server_persistent_connections %s\n", value);
	if( i == 32 )
	  snprintf(opt_line, 1000, "persistent_connection_after_error %s\n", value);
	if( i == 33 )
	  snprintf(opt_line, 1000, "detect_broken_pconn %s\n", value);
	if( i == 34 )
	  snprintf(opt_line, 1000, "balance_on_multiple_ip %s\n", value);
	if( i == 35 )
	  snprintf(opt_line, 1000, "pipeline_prefetch %s\n", value);
	if( i == 36 )
	  snprintf(opt_line, 1000, "request_entities %s\n", value);
	if( i == 37 )
	  snprintf(opt_line, 1000, "ie_refresh %s\n", value);
	if( i == 38 )
	  snprintf(opt_line, 1000, "vary_ignore_expire %s\n", value);
	if( i == 39 )
	  snprintf(opt_line, 1000, "relaxed_header_parser %s\n", value);
	if( i == 40 )
	  snprintf(opt_line, 1000, "retry_on_error %s\n", value);
	if( i == 41 )
	  snprintf(opt_line, 1000, "wccp2_rebuild_wait %s\n", value);
	if( i == 42 )
	  snprintf(opt_line, 1000, "digest_generation %s\n", value);

	/* Add combo box config values to the options buffer */
	strcat(opt_buffer, opt_line);
    }

    free(opt_line);


//    config = allocate(OPTION_BUFFER_MAX_SIZE + SERVER_BUFFER_MAX_SIZE + ACL_BUFFER_MAX_SIZE);

    /* Put all servers into the global server_buffer (file_size+new servers) */
    server_buffer = allocate(SERVER_BUFFER_MAX_SIZE);
    /* Call foreach on the acl treeview with acl_type=0 to get all ACL's */
    acl_type = 0;
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(widgets->server_treeview));
    gtk_tree_model_foreach(GTK_TREE_MODEL(model),
                          (GtkTreeModelForeachFunc) &add_servers_to_buffer, widgets);


    /* Put all ACL's into the global acl_buffer (file_size+new acls) */
    acl_buffer = allocate(ACL_BUFFER_MAX_SIZE);

    /* Call foreach on the acl treeview with acl_type=0 to get all ACL's */
    acl_type = 0;
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(widgets->acl_treeview));
    gtk_tree_model_foreach(GTK_TREE_MODEL(model),
                          (GtkTreeModelForeachFunc) &add_acls_to_buffer, widgets);

    /* Add a newline between ACL's and ACL settings */
    strcat(acl_buffer, "\n");


    /* Call foreach on the aclperms treeview 2 times with acl_type=1 to get all ACL settings */
    /* The acl_perm is used set correct "allow" then "deny" ordering */
    acl_type = 1;
    for(acl_perm=0; acl_perm<2; acl_perm++)
    {
	/* Call foreach on the aclperm treeview */
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widgets->aclperm_treeview));
	gtk_tree_model_foreach(GTK_TREE_MODEL(model),
                          (GtkTreeModelForeachFunc) &add_acls_to_buffer, widgets);
    }
    /* All ACL's have been put into the acl_buffer */


    /* FIX: Later if needed. Also get all commented lines */
//    if((fp=fopen(SQUID_CONF, "r"))==NULL)
//    {
//	free(config);
//	return;
//    }


/* Keep for future optimizations...
gint len = strlen(server_buffer);
printf("serv len: %d\n", len);   23
len = strlen(opt_buffer);
printf("opts len: %d\n", len); 3864
len = strlen(acl_buffer);
printf("ACL len: %d\n", len);  1083
*/

    config = allocate(1024 + OPTION_BUFFER_MAX_SIZE + SERVER_BUFFER_MAX_SIZE + ACL_BUFFER_MAX_SIZE);

    /* Put all servers first in the configuration */
    strcpy(config, server_buffer);
    free(server_buffer);

    /* Put all options next in the configuration */
    strcat(config, "\n");
    strcat(config, opt_buffer);
    free(opt_buffer);

    /* Put all ACL's next in the configuration */
    strcat(config, "\n");
    strcat(config, acl_buffer);
    free(acl_buffer);


//    printf("\n\nConfiguration:\n%s\n", config);

    /* Save the new configuration */
    if((fp=fopen(SQUID_CONF, "w+"))==NULL)
    {
	free(config);
	return;
    }
    fputs(config, fp);
    fclose(fp);
    free(config);


    /* Populate the server treeview */
    populate_servers(widgets);

    /* Select the first server and set global server values */    
    select_first_server(widgets);

    /* Populate the server settings */    
    populate_server_settings(widgets);

    /* Populate ACL's */
    populate_acl_tab(widgets);

    /* Populate the configuration tab */
    populate_conf_tab(widgets);

    if( activated )
      reread_conf(widgets);
}
