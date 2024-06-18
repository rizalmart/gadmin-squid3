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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include "gettext.h"
#include "widgets.h"
#include "allocate.h"
#include "show_info.h"
#include "commands.h"
#include "generate_cert.h"


void generate_cert(struct w *widgets)
{
    FILE *fp;
    char *command;
    char *pem_path;
    gdouble val=0.0;
    int i = 0, sleepsec = 1;
    gchar *info;
    
    G_CONST_RETURN gchar *ip;
    G_CONST_RETURN gchar *email;
    G_CONST_RETURN gchar *bits;
    G_CONST_RETURN gchar *password;
    G_CONST_RETURN gchar *challenge;
    G_CONST_RETURN gchar *valid;
    G_CONST_RETURN gchar *country;
    G_CONST_RETURN gchar *state;
    G_CONST_RETURN gchar *city;
    G_CONST_RETURN gchar *org;
    G_CONST_RETURN gchar *org_unit;
    G_CONST_RETURN gchar *cert_filepath;

    cert_filepath = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[15]));
    ip = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[16]));
    email = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[17]));
    state = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[18]));
    city = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[19]));
    org = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[20]));
    org_unit = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[21]));
    password = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[22]));
    challenge = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[23]));
    country = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_entry[24]));

    bits = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_spinbutton[11]));
    valid = gtk_entry_get_text(GTK_ENTRY(widgets->server_set_spinbutton[12]));

    /* Show error if a value is missing */
    if( strlen(ip) == 0 || strlen(email) == 0 || strlen(bits) == 0 || strlen(password) == 0
    ||  strlen(challenge) == 0 || strlen(valid) == 0 || strlen(country) == 0 || strlen(state) == 0
    ||  strlen(city) == 0 || strlen(org) == 0 || strlen(org_unit) == 0 || strlen(cert_filepath) < 3 )
    {
	info = g_strdup_printf(_("Every certificate field must be filled in.\n"));
	show_info(info);
	g_free(info);
	return;
    }

    /* The password field cant contain spaces */
    for(i=0; password[i]!='\0'; i++)
    if( password[i]==' ' || password[i]=='\n' || password[i]=='\r' || password[i]=='\t' )
    {
	info = g_strdup_printf(_("The password field cant contain any spaces or newline chars.\n"));
	show_info(info);
	g_free(info);
	return;       
    }


    val=0.0;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    pem_path = allocate(4096);
    snprintf(pem_path, 1024, "%s", cert_filepath);

    /* Make a new gprotls.conf with the chosen values. FIXME: one for each server */
    command = allocate(4096);
    snprintf(command, 1024, "%s/gprotls.conf", APPCONFDIR);
    if((fp=fopen(command, "w+"))==NULL)
    {
	printf("Error writing gprotls.conf here: %s\n", command);
	info = g_strdup_printf(_("Error writing gprotls.conf here: %s\n"), command);
	show_info(info);
	g_free(info);
	free(command);
	free(pem_path);
	return;
    }

    val=0.1;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    fputs("[ req ]\n", fp);
    fputs("default_bits       = ", fp);
    fputs(bits, fp); 				
    fputs("\n", fp);
    fputs("default_days       = ", fp);
    fputs(valid, fp); 				
    fputs("\n", fp);
    fputs("distinguished_name = req_distinguished_name\n", fp);
    fputs("prompt             = no\n", fp);
    fputs("output_password    = ", fp);
    fputs(password, fp);			       
    fputs("\n", fp);
    fputs("\n[ req_distinguished_name ]\n", fp);
    fputs("C            = ", fp);
    fputs(country, fp);			      
    fputs("\nST           = ", fp);
    fputs(state, fp);          
    fputs("\nL            = ", fp);
    fputs(city, fp);            
    fputs("\nO            = ", fp);
    fputs(org, fp);            
    fputs("\nOU           = ", fp);
    fputs(org_unit, fp);     
    fputs("\nCN           = ", fp);
    fputs(ip, fp);      
    fputs("\nemailAddress = ", fp);
    fputs(email, fp);  
    fputs("\n", fp);
    fputs("\n[ req_attributes ]", fp);
    fputs("\nchallengePassword = ", fp);
    fputs(challenge, fp);
    fputs("\n", fp);
    fclose(fp);


    val=0.2;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();
    
    /* Chmod_gprotls_conf_after_write */
    snprintf(command, 1024, "chmod 600 %s/gprotls.conf", APPCONFDIR);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }


    val=0.3; /* Can take some time depending on the selected number of bits */
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), _("Creating private cert and key"));
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();
    
    /* Trusted cert and private key */
    strcpy(command, "openssl req -new -newhdr -out ");
    strcat(command, APPCONFDIR);
    strcat(command, "/cert.csr -keyout ");
    strcat(command, APPCONFDIR);
    strcat(command, "/squid_privkey.pem -config ");
    strcat(command, APPCONFDIR);
    strcat(command, "/gprotls.conf");
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }



    val=0.4;
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), "Creating squid key");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    /* squid key */
    snprintf(command, 4000, "openssl rsa -in %s/squid_privkey.pem -out %s/squid.key -passin pass:%s", 
	     APPCONFDIR, APPCONFDIR, password);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }
    sleep(sleepsec);


    val=0.5;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    /* Dump key */
    strcpy(command, "openssl rsa -in ");
    strcat(command, APPCONFDIR);
    strcat(command, "/squid.key -out ");
    strcat(command, APPCONFDIR); 
    strcat(command, "/dump.key");     
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }



    val=0.6;
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), "Creating squid cert");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    /* squid cert */
    strcpy(command, "openssl x509 -in ");
    strcat(command, APPCONFDIR);
    strcat(command, "/cert.csr -out ");
    strcat(command, APPCONFDIR);
    strcat(command, "/squid.cert -req -signkey ");
    strcat(command, APPCONFDIR);
    strcat(command, "/dump.key -days ");
    strcat(command, valid);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }
    sleep(sleepsec);



    val=0.7;
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), "Adding cert text");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    /* Text output */
    strcpy(command, "openssl req -newhdr -in ");
    strcat(command, APPCONFDIR);
    strcat(command, "/cert.csr -text -out ");
    strcat(command, pem_path);
    strcat(command, " -config ");    
    strcat(command, APPCONFDIR);
    strcat(command, "/gprotls.conf");     
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }
    sleep(sleepsec);



    val=0.8;
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), "Combining certs");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    /* Combine cert with pem */
    strcpy(command, "cat ");
    strcat(command, APPCONFDIR);
    strcat(command, "/squid.cert >> ");
    strcat(command, pem_path);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }
    /* Combine key with pem */
    strcpy(command, "cat ");
    strcat(command, APPCONFDIR);
    strcat(command, "/squid.key >> ");
    strcat(command, pem_path);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }
    sleep(sleepsec);



    /* Chmod_the_certificate */
    snprintf(command, 1024, "chmod 600 %s", pem_path);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }



    val=0.9;
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), "Cleaning up");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
    while(gtk_events_pending())
	  gtk_main_iteration();

    /* Remove dump key */
    snprintf(command, 1024, "rm -f %s/dump.key", APPCONFDIR);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }

    /* Remove priv key */
    snprintf(command, 1024, "rm -f %s/squid_privkey.pem", APPCONFDIR);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }

    /* Remove cert csr */
    snprintf(command, 1024, "rm -f %s/cert.csr", APPCONFDIR);
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }

    /* Remove squid key */
    snprintf(command, 1024, "rm -f %s/squid.key", APPCONFDIR);     
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }

    /* Remove squid cert */
    snprintf(command, 1024, "rm -f %s/squid.cert", APPCONFDIR);     
    if( ! run_command(command) )
    {
	free(command);
	free(pem_path);
	return;
    }
    sleep(sleepsec);
    


    if( file_exists(pem_path) )
    {
	val=1.0;
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), "Certificate creation successful");
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
	while(gtk_events_pending())
	  gtk_main_iteration();

	info = g_strconcat(
	"\n",
	_("The certificate has been created successfully\n"),
	_("and placed in the correct location.\n"),
	_("It will be used by new FTP-TLS logins.\n"),
	"\n", NULL);
	show_info(info);
	g_free(info);
    }
    else
      {
	 val=0.0;
	 gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), "Certificate creation failed");
	 gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widgets->gen_cert_progressbar), val);
	 while(gtk_events_pending())
	   gtk_main_iteration();
    	 
	 info = g_strdup_printf(_("The certificate has not been created, something went wrong.\n"));
	 show_info(info);
	 g_free(info);
      }
      
    free(command);
    free(pem_path);    
}
