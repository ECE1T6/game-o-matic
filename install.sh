#!/bin/bash
#Installs pong to the init.d directory, to be started at boot.
#NEEDS TO BE RUN WITH SUDO PRIVELEDGES

cd hidev
sudo mv displayTest /etc/init.d/
sudo chmod +x /etc/init.d/displayTest
sudo update-rc.d displayTest defaults 

echo "Installed."
exit 1
