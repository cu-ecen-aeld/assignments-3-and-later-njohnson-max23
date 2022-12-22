#!/bin/bash

# writer.sh
# Author: Nathan Johnson
# This script creates the specified file (or replaces an existing file) containing 
# the specified text string.
# Usage: writer.sh <writefile> <writestr> where 
# <writefile> = file to create or replace
# <writestr> = string to write

if [ $# -lt 2 ]
then
    echo "Missing arguments: usage writer.sh <writefile> <writestr>"
    exit 1
fi

writefile=$1
writestr=$2

touch "$writefile"

if [ -w "$writefile" ]
then
    echo "$writestr" > "$writefile"
else
    echo "File "$writefile" could not be written"
fi
