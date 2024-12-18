#include <vector>
#include <fcntl.h> 
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h> // For mmap, munmap, msync
#include <cstring>    // For memset, strcpy
#include <fstream>
#include "OperationalSystemDescriptor.hpp"
#include "DiskManager.hpp"


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
    mapMemory(0);
}

DiskManager::DiskManager(const char* disk_manager_file, const char* disk_data_file){
    abrirArquivo(disk_data_file);
    loadDiskMetaData(disk_manager_file);
    mapMemory(0);


}

void DiskManager::abrirArquivo(const char* file_path){
    arquivo = open(file_path, O_RDWR | O_CREAT, 0600);
    if (arquivo == -1) {
        std::cerr << "Falha ao abrir o arquivo: " << strerror(errno) << std::endl;
        return;
    }
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
    quantidade_de_blocos = ((espaco_de_memoria * MEGA_BYTE)/tamanho_do_bloco) + 1;
}

void DiskManager::setVetorAlocacao(){
    vetor_alocacao.assign(quantidade_de_blocos,true); // inicia geral livre
}

void DiskManager::setVetorEspaco(){
    vetor_espaco.assign(quantidade_de_blocos,0);
}

Endereco DiskManager::memoryAlloc(unsigned int tamanho){
    Endereco endereco = procuraEndereco(tamanho);
    if (!(endereco == quantidade_de_blocos + 1)){
        preencheEndereco(endereco,tamanho);
    }

    return endereco;

}

void DiskManager::memoryDisalloc(Endereco endereco){
    unsigned short tamanho = vetor_espaco[endereco];
    for ( int i = endereco; i < (endereco + tamanho); i ++){
        vetor_alocacao[i] = true;
    }
    vetor_espaco[endereco] = 0;

}

void  DiskManager::preencheEndereco(Endereco endereco, unsigned int tamanho){
    for ( int i = endereco; i < (endereco + tamanho); i ++){
        vetor_alocacao[i] = false;
    }
    vetor_espaco[endereco] = tamanho;
}


Endereco DiskManager::procuraEndereco(unsigned int tamanho){
    bool espaco_livre = true;
    Endereco endereço; 
    Endereco posicao = prox_endereco_de_procura;


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
        std::cerr << "Memoria cheia libere espaço" << std::endl;
        return quantidade_de_blocos + 1;
    }

}


const std::vector<bool>& DiskManager::getVetorAlocacao() const{
    return vetor_alocacao;
};

const std::vector<unsigned short int>& DiskManager::getVetorEspaco() const{
    return vetor_espaco;
};

Endereco DiskManager::getProxEnderecoDeProcura() const{
    return prox_endereco_de_procura;
};


void* DiskManager::read(Endereco endereco){
        
    if (vetor_alocacao[endereco]){
        std::cerr << "Erro: endereço não alocado\n";
        std::cout << vetor_alocacao[endereco] << std::endl;
        return nullptr; // não escreve em endereços não alocados
    }
   
    Endereco endereco_real = (endereco-ultimo_map) * tamanho_do_bloco;

   

    if (endereco >= ultimo_map+tamanho_do_map or endereco < ultimo_map){
        if (msync(mapped_memory,tamanho_do_ultimo_map_bytes, MS_SYNC) == -1) {
            std::cerr << "Erro: msync falhou: " << strerror(errno) << std::endl;
        }
        if (munmap(mapped_memory, tamanho_do_ultimo_map_bytes) == -1) {
            std::cerr << "Erro: munmap falhou: " << strerror(errno) << std::endl;
        }
        mapMemory(endereco);

        endereco_real = (endereco-ultimo_map) * tamanho_do_bloco;
        void* copy = malloc(tamanho_do_bloco);

        if (endereco_real > std::numeric_limits<off_t>::max()) {
            std::cerr << "Erro: overflow de endereço na leitura\n";
            std::cout << endereco_real << std::endl;
            return nullptr;
        }


        if (copy == nullptr) {
                std::cerr << "Falha ao alocar memória\n";
                return nullptr;
            }

        memcpy(copy, static_cast<char*>(mapped_memory) + endereco_real, tamanho_do_bloco);


        return copy;


    }else{
        if (endereco_real > std::numeric_limits<off_t>::max()) {
            std::cerr << "Erro: overflow de endereço na leitura\n";
            std::cout << endereco_real << std::endl;
            return nullptr;
        }
    
        void* copy = malloc(tamanho_do_bloco);

        if (copy == nullptr) {
                std::cerr << "Falha ao alocar memória\n";
                return nullptr;
            }

        memcpy(copy, static_cast<char*>(mapped_memory) + endereco_real, tamanho_do_bloco);


        return copy;
    }

}

unsigned long long DiskManager::getQuantidadeDeBlocos() const{
    return quantidade_de_blocos;
}

unsigned int DiskManager::getTamanhoDoBloco() const{    
    return tamanho_do_bloco;
}

void DiskManager::mapMemory(Endereco endereco){
    Endereco endereco_real = endereco * tamanho_do_bloco;
    unsigned long long tamanho_em_bytes; 


    if ((quantidade_de_blocos - endereco) < tamanho_do_map ){
        tamanho_em_bytes = (quantidade_de_blocos - endereco) * tamanho_do_bloco;
    }else{
        tamanho_em_bytes = tamanho_do_bloco*tamanho_do_map;
    }
    tamanho_do_ultimo_map_bytes = tamanho_em_bytes;


    void* mapped_addr = mmap(NULL, tamanho_em_bytes, PROT_READ | PROT_WRITE,
                            MAP_SHARED, arquivo, endereco_real);
    if (mapped_addr == MAP_FAILED) {
        std::cerr << "mmap falhou: " << strerror(errno) << std::endl;
        return; 
    }
    mapped_memory = mapped_addr;

    ultimo_map = endereco;
}

void DiskManager::sincronizar(){
    if (msync(mapped_memory, tamanho_do_ultimo_map_bytes, MS_SYNC) == -1) {
        std::cerr << "Erro: msync falhou: " << strerror(errno) << std::endl;
    }

}



void DiskManager::saveDiskMetaData(const char* disk_manager_file) {
    std::ofstream file(disk_manager_file, std::ios::binary);
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo" << std::endl;
        return;
    }

    // Size of vetor_alocacao and vetor_espaco
    size_t tamanho_alocacao = vetor_alocacao.size();
    size_t tamanho_espaco = vetor_espaco.size();

    // Write fixed-size variables
    if (!file.write(reinterpret_cast<const char*>(&espaco_de_memoria), sizeof(espaco_de_memoria))) {
        std::cerr << "Erro ao escrever espaco_de_memoria" << std::endl;
        return;
    }
    if (!file.write(reinterpret_cast<const char*>(&tamanho_do_bloco), sizeof(tamanho_do_bloco))) {
        std::cerr << "Erro ao escrever tamanho_do_bloco" << std::endl;
        return;
    }
    if (!file.write(reinterpret_cast<const char*>(&quantidade_de_blocos), sizeof(quantidade_de_blocos))) {
        std::cerr << "Erro ao escrever quantidade_de_blocos" << std::endl;
        return;
    }
    if (!file.write(reinterpret_cast<const char*>(&prox_endereco_de_procura), sizeof(prox_endereco_de_procura))) {
        std::cerr << "Erro ao escrever prox_endereco_de_procura" << std::endl;
        return;
    }
    if (!file.write(reinterpret_cast<const char*>(&tamanho_do_map), sizeof(tamanho_do_map))) {
        std::cerr << "Erro ao escrever tamanho_do_map" << std::endl;
        return;
    }

    // Write size of vetor_alocacao
    if (!file.write(reinterpret_cast<const char*>(&tamanho_alocacao), sizeof(tamanho_alocacao))) {
        std::cerr << "Erro ao escrever tamanho_alocacao" << std::endl;
        return;
    }

    // Prepare the byte array for vetor_alocacao
    size_t numBytes = (tamanho_alocacao + 7) / 8; // Round up to the nearest byte
    std::vector<uint8_t> byteArray(numBytes, 0);

    for (size_t i = 0; i < tamanho_alocacao; ++i) {
        if (vetor_alocacao[i]) {
            byteArray[i / 8] |= (1 << (i % 8)); // Set the corresponding bit
        }
    }

    // Write the byte array
    if (!file.write(reinterpret_cast<const char*>(byteArray.data()), numBytes)) {
        std::cerr << "Erro ao escrever vetor_alocacao" << std::endl;
        return;
    }

    // Write size of vetor_espaco
    if (!file.write(reinterpret_cast<const char*>(&tamanho_espaco), sizeof(tamanho_espaco))) {
        std::cerr << "Erro ao escrever tamanho_espaco" << std::endl;
        return;
    }

    // Write vetor_espaco
    if (!file.write(reinterpret_cast<const char*>(vetor_espaco.data()), tamanho_espaco * sizeof(unsigned short int))) {
        std::cerr << "Erro ao escrever vetor_espaco" << std::endl;
        return;
    }
    // Close the file
    file.close();
}

void DiskManager::loadDiskMetaData(const char* disk_manager_file){
    std::ifstream file(disk_manager_file, std::ios::binary);
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo" << std::endl;
        return;
    }

    // Read fixed-size variables
    file.read(reinterpret_cast<char*>(&this->espaco_de_memoria), sizeof(this->espaco_de_memoria));
    file.read(reinterpret_cast<char*>(&this->tamanho_do_bloco), sizeof(this->tamanho_do_bloco));
    file.read(reinterpret_cast<char*>(&this->quantidade_de_blocos), sizeof(this->quantidade_de_blocos));
    file.read(reinterpret_cast<char*>(&this->prox_endereco_de_procura), sizeof(this->prox_endereco_de_procura));
    file.read(reinterpret_cast<char*>(&this->tamanho_do_map), sizeof(this->tamanho_do_map));

    // Read size of vetor_alocacao
    size_t tamanho_alocacao;
    file.read(reinterpret_cast<char*>(&tamanho_alocacao), sizeof(tamanho_alocacao));

    // Read vetor_alocacao
    size_t numBytes = (tamanho_alocacao + 7) / 8; // Round up to the nearest byte
    std::vector<char> byteArray(numBytes);
    file.read(byteArray.data(), numBytes);

    this->vetor_alocacao.resize(tamanho_alocacao);
    for (size_t i = 0; i < tamanho_alocacao; ++i) {
        this->vetor_alocacao[i] = (byteArray[i / 8] & (1 << (i % 8))) != 0;
    }
    // Read size of vetor_espaco
    size_t tamanho_espaco;
    file.read(reinterpret_cast<char*>(&tamanho_espaco), sizeof(tamanho_espaco));

    // Read vetor_espaco
    this->vetor_espaco.resize(tamanho_espaco);
    file.read(reinterpret_cast<char*>(this->vetor_espaco.data()), tamanho_espaco * sizeof(unsigned short int));

    // Close the file
    file.close();
}