#!/bin/sh
sysctl -w net.ipv4.conf.all.arp_ignore=0
sysctl -w net.ipv4.conf.all.arp_accept=1

echo "IP address of the docker is:"
ifconfig eth0 | grep 'inet ' | awk '{print $2}'

echo "MAC address of the docker is:"
ifconfig eth0 | grep 'ether' | awk '{print $2}'

echo "You have 10 seconds to set up the script before arp requests are being sent"
sleep 10

/usr/sbin/arping -c 15 -I eth0 "${TARGET_IP}"

echo "Finished arping"

arp -a

sleep infinity
