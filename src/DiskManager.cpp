#include "DiskManager.hpp"
#include <vector>
#include <fcntl.h> 
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h> // For mmap, munmap, msync
#include <cstring>    // For memset, strcpy
#include "OperationalSystemDescription.hpp"

// U significa unsigned int
#define KILO_BYTE (1024U)
#define MEGA_BYTE (KILO_BYTE * 1024U)


DiskManager::DiskManager(const char* file_path, unsigned int espaco_de_memoria, Device* d_bloco){
    this->file_path = file_path;
    this->espaco_de_memoria = espaco_de_memoria;
    this->tamanho_do_bloco = d_bloco->block_size;
    criarEspacoNaMemoria();
    setQuantidadeDeBlocos();
    setVetorAlocacao();
}

int DiskManager::criarEspacoNaMemoria(){
    arquivo = open(file_path, O_RDWR | O_CREAT, 0600);
    if (arquivo == -1) {
        std::cerr << "Falha ao abrir o arquivo: " << strerror(errno) << std::endl;
        return 1;
    }

    if (fallocate(arquivo, 0, 0, espaco_de_memoria * MEGA_BYTE) != 0) {
        std::cerr << "Falha ao alocar espaço: " << strerror(errno) << std::endl;
        close(arquivo);
        return 1;
    }
    std::cout << espaco_de_memoria << "MB alocados" << std::endl;

    return 0;
}

void DiskManager::setQuantidadeDeBlocos(){
    quantidade_de_blocos = (espaco_de_memoria * MEGA_BYTE)/tamanho_do_bloco;
}

void DiskManager::setVetorAlocacao(){
    vetor_alocacao.assign(quantidade_de_blocos,true); // inicia geral livre
}

void DiskManager::setVetorEspaco(){
    vetor_espaço.assign(quantidade_de_blocos,0);
}

unsigned long long DiskManager::memoryAlloc(unsigned int tamanho){
    

}

void  DiskManager::preencheEndereço(unsigned int endereço, unsigned int tamanho){
    for ( int i = endereço; i < (endereço + tamanho); i ++){
        vetor_alocacao[i] = false;
    }
    vetor_espaço[endereço] = tamanho;
}


unsigned long int DiskManager::procuraEndereço(unsigned int tamanho){
    bool espaco_livre = true;
    unsigned short int endereço; 

    
    for (unsigned int posicao = 0; posicao < vetor_alocacao.size(); posicao++ ){
        if (vetor_alocacao[posicao]){
            if (posicao + tamanho >= vetor_alocacao.size()){
            espaco_livre = false;
            break;
            }
            espaco_livre = true;

            for (unsigned int checa_posicao = 0; checa_posicao < tamanho; checa_posicao++) {
                if (!vetor_alocacao[posicao+checa_posicao]){
                    espaco_livre = false;
                    break;
                }
            }
        }else{
            espaco_livre = false;
        }
        if (espaco_livre){
            endereço = posicao;
            break;
        }
    }

    if (espaco_livre){
        preencheEndereço(endereço,tamanho);
        return endereço;
    }else{
        std::cerr << "Memoria cheia libere espaço" << strerror(errno) << "\n";
        return quantidade_de_blocos + 1;
    }

}