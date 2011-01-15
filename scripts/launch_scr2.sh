#!/bin/bash

# inside the screen window, make sure we are selected and start the program
screen -X select . && \
/usr/local/bin/launcher
