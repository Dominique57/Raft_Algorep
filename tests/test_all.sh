#!/bin/sh

# Check arguments
[ $# -lt 1 ] && echo "Missing test directory as first argument" && exit 1
[ ! -d "$1" ] && echo "First argument is not a directory" && exit 1

export NO_FORCE_REBUILD=true

for file in "$1"/*.cmd
do
  ./tests/test.sh "$file"
done