#!/bin/bash
filesdir=$1
searchstr=$2
if [ $# -lt 2 ];
then
    echo "not enough parameters"
    exit 1
fi
if [ ! -d "$filesdir" ];then 
    echo "$filesdir is not a directory"
    exit 1
fi

# Function to count matching lines in a file
count_matching_lines() {
  local file="$1"
  local count=0

  while IFS= read -r line; do
    if [[ "$line" == *"$searchstr"* ]]; then
      ((count++))
    fi
  done < "$file"

  echo "$count"
}

# Recursive function to search for files and count matching lines
search_files() {
  local dir="$1"
  local file_count=0
  local line_count=0

  # Iterate over files and directories in the current directory
  for entry in "$dir"/*; do
    if [ -f "$entry" ]; then
      ((file_count++))
      count=$(grep -c "$searchstr" "$entry")
      ((line_count += count))
    elif [ -d "$entry" ]; then
      # Recursively search subdirectories
      sub_file_count=0
      sub_line_count=0
      search_files "$entry"
      sub_file_count=$?
      sub_line_count=$?
      ((file_count += sub_file_count))
      ((line_count += sub_line_count))
    fi
  done

  echo "$file_count"
  echo "$line_count"
}

# Start the search from the provided filesdir
file_count=$(search_files "$filesdir")
total_files=$(echo "$file_count" | awk '{print $1}')
total_lines=$(echo "$file_count" | awk '{print $2}')


echo "The number of files are $file_count and the number of matching lines are $file_count in $filesdir is a directory"