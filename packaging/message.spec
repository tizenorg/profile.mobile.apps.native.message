Name:       org.tizen.message
Summary:    Message application
Version:    0.0.1
Release:    1
Group:      Applications/Messaging
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

%if "%{tizen_version}" == "3.0"
%define TIZEN_2_4_BUILD 0
%else
%define TIZEN_2_4_BUILD 1
%endif

Requires(post): /bin/touch
Requires: pkgmgr
BuildRequires:  pkgconfig(bundle)
BuildRequires:  pkgconfig(capi-appfw-application)
BuildRequires:  pkgconfig(capi-appfw-app-manager)
BuildRequires:  pkgconfig(capi-base-utils-i18n)
BuildRequires:  pkgconfig(capi-content-media-content)
BuildRequires:  pkgconfig(capi-media-image-util)
BuildRequires:  pkgconfig(capi-media-metadata-extractor)
BuildRequires:  pkgconfig(capi-media-player)
BuildRequires:  pkgconfig(capi-system-device)
BuildRequires:  pkgconfig(capi-system-info)
BuildRequires:  pkgconfig(capi-system-system-settings)
BuildRequires:  pkgconfig(capi-telephony)
BuildRequires:  pkgconfig(contacts-service2)
BuildRequires:  pkgconfig(db-util)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(elementary)
BuildRequires:  pkgconfig(ecore)
BuildRequires:  pkgconfig(ecore-input)
BuildRequires:  pkgconfig(ecore-imf)
BuildRequires:  pkgconfig(ecore-file)
BuildRequires:  pkgconfig(ecore-evas)
BuildRequires:  pkgconfig(eina)
BuildRequires:  pkgconfig(evas)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(libexif)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(libpng)
BuildRequires:  pkgconfig(notification)
BuildRequires:  pkgconfig(tapi)
BuildRequires:  pkgconfig(notification)
BuildRequires:  pkgconfig(efl-extension)
BuildRequires:  pkgconfig(capi-system-runtime-info)
BuildRequires:  pkgconfig(storage)

%if %TIZEN_2_4_BUILD == 0
BuildRequires:  pkgconfig(libtzplatform-config)
%endif
#private
BuildRequires:  pkgconfig(msg-service)
#public
BuildRequires:  pkgconfig(capi-messaging-messages)

BuildRequires:  cmake
BuildRequires:  edje-tools
BuildRequires:  gettext-tools

%description
message lite application.

%if %TIZEN_2_4_BUILD == 1
%define PREFIX           /usr/apps/%{name}
%define OPTPREFIX        /opt/usr/apps/%{name}
%define DATADIR          %{OPTPREFIX}/data
%define MANIFESTDIR      /usr/share/packages
%define SMACKDIR         /etc/smack/accesses.d
%define ICONDIR          /usr/share/icons/default/small
%else
%define PREFIX           %{TZ_SYS_RO_APP}/%{name}
%define MANIFESTDIR      %{TZ_SYS_RO_PACKAGES}
%define ICONDIR          %{TZ_SYS_RO_ICONS}/default/small
%endif
%define RESDIR           %{PREFIX}/res
%define EDJDIR           %{RESDIR}/edje
%define IMGDIR           %{RESDIR}/images
%define BINDIR           %{PREFIX}/bin
%define LIBDIR           %{PREFIX}/lib
%define LOCALEDIR        %{RESDIR}/locale

%prep
%setup -q

%build
export CXXFLAGS="$CXXFLAGS -DTIZEN_PRIVATE_API"
LDFLAGS+="-Wl,--rpath=%{PREFIX}/lib -Wl,--as-needed -Wl,--hash-style=both"; export LDFLAGS
cmake . \
    -DPREFIX=%{PREFIX}   \
    %if %TIZEN_2_4_BUILD == 1
    -DDATADIR=%{DATADIR} \
    -DSMACKDIR=%{SMACKDIR}   \
    -DTIZEN_2_4=%{TIZEN_2_4_BUILD} \
    %endif
    -DPKGDIR=%{name}     \
    -DIMGDIR=%{IMGDIR}   \
    -DEDJDIR=%{EDJDIR}   \
    -DPKGNAME=%{name}    \
    -DBINDIR=%{BINDIR}   \
    -DMANIFESTDIR=%{MANIFESTDIR}   \
    -DEDJIMGDIR=%{EDJIMGDIR}   \
    -DLIBDIR=%{LIBDIR}   \
    -DICONDIR=%{ICONDIR}   \
    -DLOCALEDIR=%{LOCALEDIR}   \
    %{?TEST: -DTEST=ON }

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
%if %TIZEN_2_4_BUILD == 1
mkdir -p %{buildroot}/%{OPTPREFIX}/shared/trusted
mkdir -p %{buildroot}/%{DATADIR}
%endif
mkdir -p %{buildroot}/%{LIBDIR}


%post
%if %TIZEN_2_4_BUILD == 1
chown -R 5000:5000 %{DATADIR}
chown -R 5000:5000 %{OPTPREFIX}/shared/trusted
if [ -f /usr/lib/rpm-plugins/msm.so ]
then
    find %{DATADIR} -exec chsmack -a 'org.tizen.message' {} \;
fi
%else
pkgdir_maker --create --pkgid=%{name}
%endif

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%dir
%if %TIZEN_2_4_BUILD == 1
%{OPTPREFIX}/shared/trusted
%{DATADIR}
%{SMACKDIR}/%{name}.efl
%endif
%{LIBDIR}
%{BINDIR}/*
%{RESDIR}/*
%{MANIFESTDIR}/*.xml
%{ICONDIR}/*
