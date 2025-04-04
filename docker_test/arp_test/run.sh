#!/bin/sh
echo "IP address of the docker is:"
ifconfig eth0 | grep 'inet ' | awk '{print $2}'

echo "MAC address of the docker is:"
ifconfig eth0 | grep 'ether' | awk '{print $2}'

echo "You have 10 seconds to set up the script before arp requests are being sent"
sleep 10

ping -c 1 "${TARGET_IP}"
echo "Finished arping"

arp -a

sleep infinity
