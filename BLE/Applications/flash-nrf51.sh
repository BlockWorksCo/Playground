#!/bin/sh

#-c "nrf51 mass_erase" \

openocd -f interface/stlink-v2.cfg -f target/nrf51.cfg \
-c "init" \
-c "halt" \
-c "nrf51 mass_erase" \
-c "program $1" \
-c "program $2" \
-c "reset" \
-c "exit"


