#!/bin/bash


timestamp=`date`

cd /home/steve/Projects/Update
rm -f find-ip-address
rm SpeedTest

speedtest-cli > SpeedTest
up=`grep Upload SpeedTest `
down=`grep Download SpeedTest `
echo $timestamp $up $down >> SpeedTestHistory
tail -20 SpeedTestHistory > RecentSpeedTestHistory

wget https://www.iplocation.net/find-ip-address
python Update.py 

scp Home.html zookooco@zookoo.com:~/www




