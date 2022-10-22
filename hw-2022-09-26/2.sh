#!/bin/bash

if [[ $# != 3 ]]
then
    echo 'this script needs 3 arguments'
    exit
fi

min=$1

if [[ $2 -lt $min ]]
then
    min=$2
fi

if [[ $3 -lt $min ]]
then
    min=$3
fi

max=$1

if [[ $2 -gt $max ]]
then
    max=$2
fi

if [[ $3 -gt $max ]]
then
    max=$3
fi

echo 'min-'${min}', max-'${max}
