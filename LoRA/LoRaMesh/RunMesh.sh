#!/bin/sh


./MeshSim.py > run.log


ag "trans.*ACK:" run.log | wc -l
ag "transmi.*hello world" run.log | wc -l

ag "node 37: Illuminator received.*ILLUM" run.log 

