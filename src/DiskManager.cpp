#include "DiskManager.hpp"
#include <vector>
#include <fcntl.h> 
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h> // For mmap, munmap, msync
#include <cstring>    // For memset, strcpy
#include "OperationalSystemDescriptor.hpp"

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
    setVetorEspaco();
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
    vetor_espaco.assign(quantidade_de_blocos,0);
}

unsigned long long DiskManager::memoryAlloc(unsigned int tamanho){
    unsigned long long endereco = procuraEndereco(tamanho);
    if (!(endereco == quantidade_de_blocos + 1)){
        preencheEndereco(endereco,tamanho);
    }

    return endereco;

}

void DiskManager::memoryDisalloc(unsigned long long endereco){
    unsigned short tamanho = vetor_espaco[endereco];
    for ( int i = endereco; i < (endereco + tamanho); i ++){
        vetor_alocacao[i] = true;
    }
    vetor_espaco[endereco] = 0;

}

void  DiskManager::preencheEndereco(unsigned int endereco, unsigned int tamanho){
    for ( int i = endereco; i < (endereco + tamanho); i ++){
        vetor_alocacao[i] = false;
    }
    vetor_espaco[endereco] = tamanho;
}


unsigned long long DiskManager::procuraEndereco(unsigned int tamanho){
    bool espaco_livre = true;
    unsigned long long endereço; 
    unsigned long long posicao = prox_endereco_de_procura;


    if ((posicao == vetor_espaco.size() - 1) or (posicao + tamanho >= vetor_espaco.size() - 1)){
        posicao = 0;
    }

    for (posicao; posicao < vetor_alocacao.size(); posicao++ ){
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
        prox_endereco_de_procura = endereço + tamanho;     
        return endereço;
    }else{
        std::cerr << "Memoria cheia libere espaço" << strerror(errno) << "\n";
        return quantidade_de_blocos + 1;
    }

}


const std::vector<bool>& DiskManager::getVetorAlocacao() const{
    return vetor_alocacao;
};

const std::vector<unsigned short int>& DiskManager::getVetorEspaco() const{
    return vetor_espaco;
};

unsigned long long DiskManager::getProxEnderecoDeProcura() const{
    return prox_endereco_de_procura;
};
