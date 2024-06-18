## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

sbin_PROGRAMS = gadmin-squid

gadmin_squid_SOURCES = \
	gadmin_squid.c gadmin_squid.h \
	create_swap_dirs.c create_swap_dirs.h \
	populate_gadmin_squid.c populate_gadmin_squid.h \
	widgets.c widgets.h \
	support.c support.h \
	create_main_window.c create_main_window.h \
	add_standard_users.c add_standard_users.h \
	create_server_tab.c create_server_tab.h \
	populate_servers.c populate_servers.h \
	server_treeview_row_clicked.c server_treeview_row_clicked.h \
	create_server_settings.c create_server_settings.h \
	populate_server_settings.c populate_server_settings.h \
	apply_server_settings.c apply_server_settings.h \
	select_first_server.c select_first_server.h \
	create_acl_tab.c create_acl_tab.h \
	populate_acl_tab.c populate_acl_tab.h \
	create_user_tab.c create_user_tab.h \
	populate_users.c populate_users.h \
	user_treeview_row_clicked.c user_treeview_row_clicked.h \
	populate_shell_combo.c populate_shell_combo.h \
	add_user.c add_user.h \
	create_user_settings.c create_user_settings.h \
	populate_user_settings.c populate_user_settings.h \
	select_first_user.c select_first_user.h \
	apply_user.c apply_user.h \
	delete_user.c delete_user.h \
	create_del_system_user_question.c create_del_system_user_question.h \
	make_settings_entries.c make_settings_entries.h \
	make_settings_combos.c make_settings_combos.h \
	make_settings_spinbuttons.c make_settings_spinbuttons.h \
	make_settings_buttons.c make_settings_buttons.h \
	make_settings_checkbuttons.c make_settings_checkbuttons.h \
	make_settings_progressbars.c make_settings_progressbars.h \
	make_settings_labels.c make_settings_labels.h \
	make_settings_hseparators.c make_settings_hseparators.h \
	create_transfer_tab.c create_transfer_tab.h \
	status_update.c status_update.h \
	create_disc_tab.c create_disc_tab.h \
	populate_disc_tab.c populate_disc_tab.h \
	create_access_tab.c create_access_tab.h \
	populate_access_tab.c populate_access_tab.h \
	clear_access_tab.c clear_access_tab.h \
	create_error_tab.c create_error_tab.h \
	populate_error_tab.c populate_error_tab.h \
	clear_error_tab.c clear_error_tab.h \
	create_cache_tab.c create_cache_tab.h \
	populate_cache_tab.c populate_cache_tab.h \
	clear_cache_tab.c clear_cache_tab.h \
	create_conf_tab.c create_conf_tab.h \
	populate_conf_tab.c populate_conf_tab.h \
	save_conf_tab.c save_conf_tab.h \
	set_version.c set_version.h \
	reread_conf.c reread_conf.h \
	apply_button_clicked.c apply_button_clicked.h \
	commands.c commands.h \
	generate_cert.c generate_cert.h \
	functions.c functions.h \
	get_option_pos.c get_option_pos.h \
	credits_window.c credits_window.h \
	activate_button_clicked.c activate_button_clicked.h \
	deactivate_button_clicked.c deactivate_button_clicked.h \
	show_info.c show_info.h \
	chars_are_digits.c chars_are_digits.h \
	allocate.c allocate.h \
	system_defines.c system_defines.h \
	standard_conf.c standard_conf.h \
	show_help.c show_help.h \
	commented.c commented.h

gadmin_squid_LDADD = @PACKAGE_LIBS@
