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



#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gtk/gtk.h>

extern struct w *widgets;

int cmplowercase(char *buf, char *cmpstring);

gchar *get_user_setting(gchar *user, gchar *what);

gchar *get_group_name(gchar *groupnr);

char *encrypt_password(G_CONST_RETURN gchar *password);

int fix_newlines_in_conf();

void fix_path(char *path);

void randomize_username(struct w *widgets);

void randomize_password(struct w *widgets);

char *random_string(int len);

int user_exists(G_CONST_RETURN gchar *username);

int group_exists(G_CONST_RETURN gchar *username);

gchar *get_user_comment(gchar *user);

void password_user(G_CONST_RETURN gchar *username, G_CONST_RETURN gchar *password);

int is_banned(char *user);

void exec_ssl(char *command, char descr[128]);

void make_dir_chmod(gchar *directory, char perm[128]);

int file_exists(char *entity);

#endif
