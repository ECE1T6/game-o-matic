#!/bin/bash
#Installs pong to the init.d directory, to be started at boot.
#NEEDS TO BE RUN WITH SUDO PRIVELEDGES

for i in {1..3}
sudo rm displayTest /etc/init.d/
sudo update-rc.d displayTest remove 

echo "Uninstalled."
exit 1
