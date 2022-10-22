#!/bin/bash

if [[ $# == 0 ]]
then
    exit
fi

args=''

for arg in $@
do
    args+=$arg
    args+='\n'
done

echo -e ${args::-2} | tac -
