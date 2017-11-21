Name:           test
Version:        1.0.0
Release:        2
Summary:        test-daemon

License:        GPL
#URL:            
Source0:        %{name}-%{version}.tar.gz

BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires:  gcc,make
#Requires:       

%description
a test file to build the rpm package

%prep

%setup -q


%build
make

%install
rm -rf $RPM_BUILD_ROOT
%{__install} -d $RPM_BUILD_ROOT/usr/sbin
%{__install} -d $RPM_BUILD_ROOT/usr/lib64
%{__install} -d $RPM_BUILD_ROOT/etc
%{__install} -d $RPM_BUILD_ROOT/usr/1452088
%{__install} -D test-1452088.service  $RPM_BUILD_ROOT/usr/lib/systemd/system/test-1452088.service

make install DESTDIR=$RPM_BUILD_ROOT


%pre

echo "准备安装test-1452088"
mkdir -p /usr/1452088

	
%post
	echo "完成安装test-1452088"

%preun
	echo  "准备卸载test-1452088"
	rm -rf /etc/1452088.conf
	rm -rf /usr/sbin/test-1452088 
	rm -rf /usr/lib64/lib1551234.so 
	rm -rf /usr/1452088/1452088.dat 
	rm -rf /usr/lib/systemd/system/test-1452088.service

%postun
	echo  "完成卸载test-1452088"
	if [ "`ls -A /usr/1452088`" = "" ]; then `rm -rf /usr/1452088`; fi

	


%clean
rm -rf $RPM_BUILD_ROOT



%files
/etc/1452088.conf
/usr/sbin/test-1452088 
/usr/lib64/lib1452088.so
/usr/1452088/1452088.dat
/usr/lib/systemd/system/test-1452088.service

%doc



%changelog
