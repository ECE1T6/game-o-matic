#!/bin/bash
#Installs pong to the init.d directory, to be started at boot.
#NEEDS TO BE RUN WITH SUDO PRIVELEDGES

cd hidev
sudo mv pong /etc/init.d/
sudo chmod +x /etc/init.d/pong 
sudo update-rc.d pong defaults 

echo "Installed."
exit 1
