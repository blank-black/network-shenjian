ODIR= $(DESTDIR)

test: lib1551265.so
	cc -g -Wall -o test test.c -L. -l1551265 -Wl,-rpath .

lib1551265.so: read.c
	cc read.c -fPIC -shared -o lib1551265.so


install:test lib1551265.so $(ODIR)
	cp test $(ODIR)/usr/sbin/test-1551265
	cp lib1551265.so $(ODIR)/usr/lib64
	mkdir -p $(ODIR)/usr/1551265
	cp 1551265.dat $(ODIR)/usr/1551265
	cp 1551265.conf $(ODIR)/etc

rpm:test-1551265.tar.bz2 test.spec
	mkdir -pv ~/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
	cp test-1551265.tar.bz2 /root/rpmbuild/SOURCES
	cp test.spec /root/rpmbuild/SPECS
	rpmbuild -ba /root/rpmbuild/SPECS/test.spec
	cp /root/rpmbuild/RPMS/x86_64/test-1551265-zby.x86_64.rpm test-1551265.rpm

uninstall:
	rm -rf /etc/1551265.conf /usr/sbin/test-1551265 /usr/lib64/lib1551265.so /usr/1551265/1551265.dat /usr/lib/systemd/system/test-1551265.service/root/rpmbuild/RPMS/x86_64/test-1551265-0.x86_64.rpm /root/rpmbuild/RPMS/x86_64/1551265-debuginfo-test-0.x86_64.rpm /root/rpmbuild
	if [ "`ls -A /usr/1551265`" = "" ]; then `rm -rf /usr/1551265`;fi;
	
clean:
	rm -rf lib1551265.so test test-1551265.rpm


