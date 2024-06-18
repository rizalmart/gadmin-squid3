%define desktop_vendor newrpms
%{!?_dist: %{expand: %%define dist rhfc14}}

Summary:    GADMIN-SQUID -- A GTK+ administation tool for the Squid proxy server.
Name:       gadmin-squid
Version:    0.1.4
Release:    0.1.%{dist}.nr
License:    GPLv3+
Group:      Applications/System
URL:        http://dalalven.dtdns.net/linux.html
Source0:    http://dalalven.dtdns.net/linux/%{name}/%{name}-%{version}.tar.gz
BuildRoot:  %{_builddir}/%{name}-%{version}-root
Provides:   gadmin-squid

Requires:   usermode >= 1.36
Requires:   squid >= 2.6
Requires:   pam
Requires:   openssl

BuildRequires: autoconf
BuildRequires: automake
BuildRequires: glib-devel
BuildRequires: gtk2-devel
BuildRequires: desktop-file-utils
BuildRequires: gettext


%description
GADMIN-SQUID is a fast and easy to use GTK+ administration tool for the Squid proxy server.

%prep
%setup -q
%configure SYSINIT_START_CMD="chkconfig squid on" SYSINIT_STOP_CMD="chkconfig squid off"

%build
%{__make}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall INSTALL_USER=`id -un` INSTALL_GROUP=`id -gn`

# pam auth
install -d %{buildroot}%{_sysconfdir}/pam.d/
install -d %{buildroot}%{_sysconfdir}/security/console.apps
install -m 644 etc/pam.d/%{name} %{buildroot}%{_sysconfdir}/pam.d/%{name}
install -m 644 etc/security/console.apps/%{name} %{buildroot}%{_sysconfdir}/security/console.apps/%{name}

# desktop entry
install -d %{buildroot}%{_datadir}/applications
install -m 644 desktop/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop

# doc files.. 
install -d %{buildroot}%{_datadir}/doc/%{name}
install -m 644  README COPYING AUTHORS ChangeLog %{buildroot}%{_datadir}/doc/%{name}

### Fixme, translations %find_lang %name

%post 
if test ! -h %{_bindir}/gadmin-squid ; then \
ln -s %{_bindir}/consolehelper %{_bindir}/gadmin-squid ; \
fi;

%clean
rm -rf $RPM_BUILD_ROOT

### Fixme, translations  %files -f %{name}.lang
%files
%defattr(0755, root, root)
%{_sbindir}/%{name}

%defattr(0644, root, root)
#%doc COPYING AUTHORS ChangeLog
%{_datadir}/doc/%{name}/README
%{_datadir}/doc/%{name}/COPYING 
%{_datadir}/doc/%{name}/AUTHORS 
%{_datadir}/doc/%{name}/ChangeLog

%{_sysconfdir}/pam.d/%{name}
%{_sysconfdir}/security/console.apps/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/*.png
%dir %{_datadir}/pixmaps/%{name}
%{_datadir}/pixmaps/%{name}/*.png

%changelog
* Mon Jun 04 2012 Magnus-swe <magnus-swe@telia.com>
- Changed Credits to About.
- Fixes a possible startup crash.
* Wed Apr 07 2010 Magnus-swe <magnus-swe@telia.com>
- Added activation status colors.
- Uses CIDR masks instead of deprecated ACL netmasks.
- Merges 7 patches and a new specfile from
- S.A. Hartsuiker <s.a.hartsuiker@gmail.com>
- Does not add duplicate ACL names to the second
- ACL permission names combo.
* Tue Nov 10 2009 Magnus-swe <magnus-swe@telia.com>
- Makes automatic squid.conf backups for each new version.
- Now using "squid -k kill" to deactivate the server.
- All options are ok for Fedora 11, Debian and Ubuntu.
- Removes ACL name 'all' as an ACL definition type.
- Added unit descriptions to some options that now require them.
- Removes unhandled extra options.
- Removes more options that Debian/Ubuntu's Squid is not compiled with:
- incoming_icp_average, incoming_http_average, incoming_dns_average
- min_icp_poll_cnt, min_dns_poll_cnt, min_http_poll_cnt.
* Mon Jul 20 2009 Magnus-swe <magnus-swe@telia.com>
- Fixed array comparizon warnings.
- Removed obsoleted ACL name 'all' as its now built in.
- Removed seldom used options.
* Tue Feb 19 2008 Magnus-swe <magnus-swe@telia.com>
- Changed name to GAdmin-Squid.
- Switched to GPL version 3.
* Mon Jun 25 2007 Magnus-swe <magnus-swe@telia.com>
- Initial release.
