#!/bin/bash

# Everything is gonna be run from outside the plotter

if [ "$1" == "batch" ]; then
    source plotter/t4/batch.sh

elif [ "$1" == "yields" ]; then    
    root -l -b plotter/t4/yields.C\(\"~/www/t4/plots\"\)

else
    echo "Es que no me hás dicho que hacer, así que mejor si me duermo."

fi