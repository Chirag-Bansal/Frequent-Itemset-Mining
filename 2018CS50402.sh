#!/bin/bash
if [ -f apriori ] && [ -f fptree ]; then
    if [ "$#" == 4 ] && [ "$3" == "-apriori" ]; then
        ./apriori "$1" "$2" "$4".txt
    elif [ "$#" == 4 ] && [ "$3" == "-fptree" ]; then
        ./fptree "$1" "$2" "$4".txt
    elif [ "$#" == 2 ] && [ "$2" == "-plot" ]; then
        python3 plot.py "$1"
    else
        echo "Illegal number of parameters ($#), should be 4"
    fi
else
    echo "Executable not found, run compile.sh first"
fi 