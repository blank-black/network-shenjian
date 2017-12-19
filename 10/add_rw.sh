#!/bin/bash
#$1:创建允许rw的smb用户

chmod 755 /home/$1

echo "[$1]
        comment = the $1 folder
        path = /home/$1
        public = no
        read only = no
        create mode = 0777
		directory mode = 0777
		valid users = root $1
		write list = root $1
		read list = root $1

">>/etc/samba/smb.conf	

echo "`date +%Y.%m.%d--%H:%M:%S`   Root add $1 read and write permissions.">>/log/$1.log

systemctl restart smb.service