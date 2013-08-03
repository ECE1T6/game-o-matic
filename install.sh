#!/bin/bash
#Installs pong to the init.d directory, to be started at boot.
#NEEDS TO BE RUN WITH SUDO PRIVELEDGES

cd lodev
sudo mv run /etc/init.d/gameomatic
sudo chmod +x /etc/init.d/gameomatic
sudo update-rc.d gameomatic defaults 

echo "Installed."
exit 0
