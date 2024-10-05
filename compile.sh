#!/usr/bin/bash

#Criando os .o dos src


SRC_FILES=(./src/*.cpp)


if [ -d ./src  ]
then
  :
else
  echo "Diretorio /src não encontrado"
  exit 1
fi

if [ "${#SRC_FILES[@]}" -eq 0 ]; then
    echo "Nenhum arquivo .cpp encontrado em ./src."
    exit 1
fi



if [ -d ./obj  ]
then
  :
else
  echo "Diretorio /obj não encontrado"
  exit 1
fi



for file in "${SRC_FILES[@]}"
do  
    g++ -c "$file" -o "./obj/$(basename "$file" .cpp).o"
    if [ $? -eq 0 ]
    then 
        echo "$file compilado com sucesso"
    else 
        exit 1
    fi

done



if [ -z "$1" ]
    then 
        g++ ./obj/*.o -o "./bin/my_project"
        echo "PROJECT_NAME=my_project" > project.config
    else 
        g++ ./obj/*.o -o "./bin/$1"     
        echo "PROJECT_NAME=$1" > project.config
fi

