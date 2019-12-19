#!/bin/sh


openocd -f interface/stlink-v2.cfg -f target/nrf51.cfg \
-c "init" \
-c "halt"


