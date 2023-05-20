#!/bin/bash

#for file in ./*; do
#    sed -i 's/include <libcmaes\/\(.*\.h\)>/include "\1"/' "$file"
#done

find . -type f -name "*" -exec sed -i 's/include <libcmaes\/\(.*\.h\)>/include "\1"/' {} +
