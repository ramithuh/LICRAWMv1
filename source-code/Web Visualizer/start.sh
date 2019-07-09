#!/bin/bash
node p5.serialport-master/startserver.js &
cd 2D\ Visualizer
python -m SimpleHTTPServer 8880
#http-server