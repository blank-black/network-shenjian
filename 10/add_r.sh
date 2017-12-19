#!/bin/bash

mkdir /home/$1
chmod 755 /home/$1

echo "[$1]
        comment = the $1 groups work directory
        path = /home/$1
        public = no
        read only = no
        create mode = 0777
		directory mode = 0777
		valid users = root $1
		write list = root
		read list = root $1

">>/etc/samba/smb.conf	

testparm

echo "ÇëÊäÈë$1ÃÜÂë"
smbpasswd -a $1

systemctl restart smb.service
