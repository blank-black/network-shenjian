#!/bin/bash
#$1 待修改目录权限用户名
#$2 添加可rw访问$1的用户

temp=`sed "/\[$1\]/"q /etc/samba/smb.conf|sed -n '$='`

let temp+=7
sed -i "${temp}s/$/ $2/" /etc/samba/smb.conf

let temp++
sed -i "${temp}s/$/ $2/" /etc/samba/smb.conf

let temp++
sed -i "${temp}s/$/ $2/" /etc/samba/smb.conf

echo "`date +%Y.%m.%d--%H:%M:%S`   Root add $2 read and write permissions.">>/log/$1.log

systemctl restart smb.service