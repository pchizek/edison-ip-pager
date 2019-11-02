#!/bin/bash

# Usage check
if [ $# = 2 ]; then

    name="$1"
    val="$2"

    declare "nm"="$1"
    declare "$nm"="$2"

    export $nm

else
    echo "Error: invalid number of arguments"

fi
