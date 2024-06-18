## Process this file with automake to produce Makefile.in

VERSION = @VERSION@

SUBDIRS = src po

EXTRA_DIST = \
	autogen.sh

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	   $(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/gadmin-squid; \
	   for pixmap in $(srcdir)/pixmaps/*; do \
	     if test -f $$pixmap; then \
	        $(INSTALL_DATA) $$pixmap $(DESTDIR)$(datadir)/pixmaps/gadmin-squid; \
	     fi \
	   done \
	fi;
	if test -d $(DESTDIR)$(datadir)/pixmaps; then \
	   cp pixmaps/gadmin-squid.png $(DESTDIR)$(datadir)/pixmaps/; \
	fi;
	if test -f $(DESTDIR)$(bindir)/consolehelper && test ! -f $(DESTDIR)$(bindir)/gadmin-squid; then \
	   ln -s $(DESTDIR)$(bindir)/consolehelper $(DESTDIR)$(bindir)/gadmin-squid; \
	fi;
	@echo "Failsafe for a mixed environment"
	if test -f $(DESTDIR)/usr/local/bin/consolehelper && test ! -f $(DESTDIR)/usr/local/bin/gadmin-squid; then \
	   ln -s $(DESTDIR)/usr/local/bin/consolehelper $(DESTDIR)/usr/local/bin/gadmin-squid; \
	fi;
	if test -d $(DESTDIR)$(sysconfdir)/pam.d; then \
	   cp etc/pam.d/gadmin-squid $(DESTDIR)$(sysconfdir)/pam.d/; \
	fi;
	if test -d $(DESTDIR)$(sysconfdir)/security/console.apps; then \
	   cp etc/security/console.apps/gadmin-squid $(DESTDIR)$(sysconfdir)/security/console.apps/; \
	fi;   
	if test -d $(DESTDIR)$(datadir)/applications; then \
	   cp desktop/gadmin-squid.desktop $(DESTDIR)$(datadir)/applications/; \
	fi;
	if test -d $(DESTDIR)$(datadir)/gnome/apps/Internet; then \
	   cp desktop/gadmin-squid.desktop $(DESTDIR)$(datadir)/gnome/apps/Internet/; \
	fi;
	if test ! -d $(DESTDIR)$(datadir)/doc/gadmin-squid; then \
	   $(mkinstalldirs) $(DESTDIR)$(datadir)/doc/gadmin-squid; \
	fi;
	if test -d $(DESTDIR)$(docdir); then \
	   cp AUTHORS $(DESTDIR)$(docdir); \
	   cp README $(DESTDIR)$(docdir); \
	   cp ChangeLog $(DESTDIR)$(docdir); \
	   cp COPYING $(DESTDIR)$(docdir); \
	fi;
	if test ! -d $(DESTDIR)$(sysconfdir)/gadmin-squid; then \
	   $(mkinstalldirs) $(DESTDIR)$(sysconfdir)/gadmin-squid; \
	fi;
	@echo ""
	@echo "Start GADMIN-SQUID by typing gadmin-squid in a terminal"
	@echo "or start it from the menu."
	@echo ""
	@echo "Send bugreports, comments or questions to magnus-swe@telia.com"
	@echo ""

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(DESTDIR)$(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(DESTDIR)$(distdir)/pixmaps; \
	    fi \
	  done \
	fi
