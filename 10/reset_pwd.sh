#!/bin/bash
#$1重置密码用户名

# echo "请输入$1密码"
smbpasswd -a $1

echo "`date +%Y.%m.%d--%H:%M:%S`   Reset password.">>/log/$1.log

systemctl restart smb.service
