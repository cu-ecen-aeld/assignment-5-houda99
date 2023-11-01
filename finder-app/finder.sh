#!/bin/bash
if [ -z "$1" ] || [ -z "$2" ]; then
    echo " Error : not enough parameters"
    exit 1
fi
filesdir=$1
searchstr=$2
if [ ! -d "$filesdir" ];then 
    echo "Error: $filesdir is not a directory"
    exit 1
fi
filescount=$(find "$filesdir" -type f| wc -l)
matchcount=$(grep -r "$searchstr" "$filesdir" | wc -l)

echo "The number of files are $filescount and the number of matching lines are $matchcount"