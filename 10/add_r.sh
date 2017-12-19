#!/bin/bash
#$1:创建的允许r的smb用户

chmod 755 /home/$1

echo "[$1]
        comment = the $1 folder
        path = /home/$1
        public = no
        read only = no
        create mode = 0777
		directory mode = 0777
		valid users = root $1
		write list = root
		read list = root $1

">>/etc/samba/smb.conf	

echo "`date +%Y.%m.%d--%H:%M:%S`   Root add $1 read permissions.">>/log/$1.log

systemctl restart smb.service
