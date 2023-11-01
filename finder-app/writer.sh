#!/bin/bash
writefile=$1
writestr=$2

if [ $# -lt 2 ];
then
    echo "not enough parameters"
    exit 1
fi
directory=$(dirname "$writefile")
mkdir -p "$directory"

# Write the content to the file
echo "$writestr" > "$writefile"

# Check if the file was created successfully
if [ $? -ne 0 ]; then
  echo "Error: Failed to create the file: $writefile"
  exit 1
fi

echo "File created successfully: $writefile"
