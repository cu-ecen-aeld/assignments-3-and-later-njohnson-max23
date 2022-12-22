#!/bin/bash

# finder.sh
# Author: Nathan Johnson
# This script counts the number of lines containing the specified search string
# in all files in the specified search directory and all subdirectories.
# Usage: finder.sh <filesdir> <searchstr> where 
# <filesdir> = directory to search
# <searchstr> = string to search for

if [ $# -lt 2 ]
then
    echo "Missing arguments: usage finder.sh <filesdir> <searchstr>"
    exit 1
fi

filesdir=$1
searchstr=$2
let total_lines=0
let total_files=0

if [ -d "$filesdir" ]
then
    for file in $(find "$filesdir" -type f)
    do
        echo "$file"
        count=$(grep -c "$searchstr" "$file")
        let total_files++
        total_lines=$((total_lines+count))
    done
else
    echo "$filesdir Is not a directory"
    exit 1
fi

echo "The number of files are $total_files and the number of matching lines are $total_lines"
