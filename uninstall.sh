#!/bin/bash
#Installs pong to the init.d directory, to be started at boot.
#NEEDS TO BE RUN WITH SUDO PRIVELEDGES

for i in {1..3}
cd /etc/init.d
sudo rm gameomatic
sudo update-rc.d gameomatic remove 

echo "Uninstalled."
exit 0
