#!/bin/bash
# filename: test.sh


mkdir /share
chmod 777 /share

echo '[share]
        path = /share
        available = yes
        browseable = yes
        public = yes
        writable = yes'>>/etc/samba/smb.conf

touch /etc/samba/smbpasswd
smbpasswd -a root

systemctl disable firewalld
systemctl stop firewalld

setsebool -P samba_enable_home_dirs on

sed -i 's/SELINUX=enforcing/SELINUX=disabled/g' /etc/selinux/config
source /etc/selinux/config

systemctl restart smb.service
systemctl enable smb.service
echo '初始化成功'