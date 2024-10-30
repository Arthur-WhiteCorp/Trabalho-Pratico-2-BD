#!/usr/bin/bash

#Criando os .o dos src
shopt -s extglob


SRC_FILES=(./src/*.cpp)
SRC_MAINS=("upload.cpp" "seek.cpp" "item3")

for main in "${SRC_MAINS[@]}"; do
    for i in "${!SRC_FILES[@]}"; do
        if [ "${SRC_FILES[i]}" = "./src/$main" ]; then
            unset "SRC_FILES[i]"
        fi
    done
done

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


if [ -z "$1" ]
    then 
        echo digite o nome do arquivo src
        echo "ex: ./compile.sh <src_name>"
        exit 1
    else 
        :
fi




for file in "${SRC_FILES[@]}"
do  
    g++ -I ./include -c "$file" -o "./obj/$(basename "$file" .cpp).o"
    if [ $? -eq 0 ]
    then 
        echo "$file compilado com sucesso"
    else 
        exit 1
    fi

done



EXE_NAME="${1%.cpp}"
g++ ./src/"$1" -I ./include ./obj/!(*upload*|*seek*).o -o ./bin/"$EXE_NAME"
echo "PROJECT_NAME=$EXE_NAME" > project.config
       


