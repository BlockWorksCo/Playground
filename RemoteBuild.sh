#!/bin/sh


rsync -avz ./ steve@192.168.2.249:~/RemoteBuild/Playground

ssh steve@192.168.2.249 "cd ~/RemoteBuild/Playground && gcc -o Hello Hello.c "

rsync -avz steve@192.168.2.249:~/RemoteBuild/Playground ./




