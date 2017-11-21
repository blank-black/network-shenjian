Name:           test
Version:        1551265
Release:        zby
Summary:        test

License:        GPL
#URL:            
Source0:        %{name}-%{version}.tar.bz2

BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires:  gcc,make
#Requires:       

%description
zby

%prep

%setup -q

%build
make

%install
rm -rf $RPM_BUILD_ROOT
%{__install} -d $RPM_BUILD_ROOT/usr/sbin
%{__install} -d $RPM_BUILD_ROOT/usr/lib64
%{__install} -d $RPM_BUILD_ROOT/etc
%{__install} -d $RPM_BUILD_ROOT/usr/1551265
%{__install} -D test-1551265.service  $RPM_BUILD_ROOT/usr/lib/systemd/system/test-1551265.service

make install DESTDIR=$RPM_BUILD_ROOT


%pre

echo 准备安装test-1551265
mkdir -p /usr/1551265

	
%post
	echo 完成安装test-1551265

%preun
	echo  准备卸载test-1551265
	

%postun
	echo  完成卸载test-1551265
	if [ "`ls -A /usr/1551265`" = "" ]; then `rm -rf /usr/1551265`; fi
	
	
%clean
rm -rf $RPM_BUILD_ROOT



%files
/etc/1551265.conf
/usr/sbin/test-1551265
/usr/lib64/lib1551265.so
/usr/1551265/1551265.dat
/usr/lib/systemd/system/test-1551265.service

