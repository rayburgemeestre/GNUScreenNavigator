#!/bin/bash
# update screen display, open launcher in seperate screen window

screen -X redisplay && \
screen -t temp /usr/local/bin/launch_scr2.sh
