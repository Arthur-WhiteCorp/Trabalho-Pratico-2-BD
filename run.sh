#!/usr/bin/bash

if [ -f project.config ]; then
    source project.config > /dev/null
else
    echo "Config file não encontrado."
    exit 1
fi



if [ -z "$1" ]
    then 
        :
    else 
        echo executando "$1"
        sudo ./bin/"$1"
        exit 0
fi



if [ -x ./bin/"$PROJECT_NAME" ]
    then 
        echo executando "$PROJECT_NAME"
        sudo ./bin/"$PROJECT_NAME"
        exit 0
fi