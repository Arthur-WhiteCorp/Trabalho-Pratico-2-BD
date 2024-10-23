#include "HashMaker.hpp"
#include <cmath>
HashMaker::HashMaker(uintmax_t tamanho_do_arquivo){
    setQuantidadeDeBlocosDeArquivo(tamanho_do_arquivo);
    setTamanhoDoHash();
}

void HashMaker::setQuantidadeDeBlocosDeArquivo(uintmax_t tamanho_do_arquivo){
    size_t tamanho = (tamanho_do_arquivo / TAMANHO_DO_BLOCO_DE_ARQUIVO) + 1;
    quantidade_de_blocos_de_arquivo = tamanho;
}

void HashMaker::setTamanhoDoHash(){
    hash = new endereco[quantidade_de_blocos_de_arquivo];
    unsigned long long memoria = sizeof(endereco) * quantidade_de_blocos_de_arquivo;
    std::cout << "este é o tamanho do hash:" <<  memoria  << std::endl;
}