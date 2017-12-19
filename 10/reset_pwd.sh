#!/bin/bash

# echo "ÇëÊäÈë$1ÃÜÂë"
smbpasswd -a $1

systemctl restart smb.service
