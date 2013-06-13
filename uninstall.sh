#!/bin/bash
#Installs pong to the init.d directory, to be started at boot.
#NEEDS TO BE RUN WITH SUDO PRIVELEDGES

for i in {1..3}
sudo rm pong /etc/init.d/
sudo update-rc.d pong remove 

echo "Uninstalled."
exit 1
