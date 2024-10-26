#ifndef DISK_MANAGER
#define DISK_MANAGER

#include <vector>
#include <fcntl.h> 
#include <iostream>
#include <sys/mman.h>
#include <cstring>
#include "OperationalSystemDescriptor.hpp"

using Endereco = unsigned long long;

class DiskManager{
private:

    const char* file_path; // como estou usando baixo nível não posso usar string
    int arquivo;// file descriptor de baxio nível deferente do file stream
    unsigned int espaco_de_memoria; // em megabytes
    unsigned int tamanho_do_bloco; // em bytes
    unsigned long long quantidade_de_blocos;
    Endereco prox_endereco_de_procura = 0; // endereco onde a procura de espaco vai começar
    std::vector <bool> vetor_alocacao; // vetor de alocação dos blocos (true livre) (false alocado)
    std::vector <unsigned short int> vetor_espaco; // vetor de espaços de endereço de memoria
    void* mapped_memory; 
    unsigned int tamanho_do_map = 300u; // tamanho em blocos 
    unsigned long long tamanho_do_ultimo_map_bytes;
    Endereco ultimo_map;

    int criarEspacoNaMemoria();
    void setVetorAlocacao();
    void setVetorEspaco();
    void setQuantidadeDeBlocos();
    void preencheEndereco(Endereco endereco, unsigned int tamanho);
    Endereco procuraEndereco(unsigned int tamanho); // de "tamanho" blocos
    void mapMemory(Endereco endereco);


public:
    
    DiskManager(const char* file_path, unsigned int espaco_de_memoria, Device* dispositivo_de_memoria);
    const std::vector<bool>& getVetorAlocacao() const;
    const std::vector<unsigned short int>& getVetorEspaco() const;
    Endereco getProxEnderecoDeProcura() const;
    Endereco memoryAlloc(unsigned int tamanho);
    void memoryDisalloc(Endereco endereco);

    unsigned long long getQuantidadeDeBlocos() const;
    unsigned int getTamanhoDoBloco() const;

    template <typename T>
    void write(Endereco endereco, T* dados);
    void* read(Endereco endereco);
    void sincronizar();


};

template <typename T>

void DiskManager::write(Endereco endereco, T* dados){


    if (vetor_alocacao[endereco]){
        std::cerr << "Erro: endereço não alocado\n";
        std::cout << vetor_alocacao[endereco] << std::endl;
        return; // não escreve em endereços não alocados
    }
    if (sizeof(T) > tamanho_do_bloco) {
        std::cerr << "Erro: T é maior que o tamanho do bloco\n";
        return;
    }


    
    if (endereco >= ultimo_map+tamanho_do_map or endereco < ultimo_map){
        if (msync(mapped_memory, tamanho_do_ultimo_map_bytes, MS_SYNC) == -1) {
            std::cerr << "Erro: msync falhou: " << strerror(errno) << std::endl;
        }
        if (munmap(mapped_memory, tamanho_do_ultimo_map_bytes) == -1) {
            std::cerr << "Erro: munmap falhou: " << strerror(errno) << std::endl;
            perror("munmap");

        }
        mapMemory(endereco);
        Endereco endereco_real = (endereco-ultimo_map) * tamanho_do_bloco;
        if (endereco_real > std::numeric_limits<off_t>::max()) {
            std::cerr << "Erro: overflow de endereço na escrita\n";
            return;
        }

        memcpy(static_cast<char*>(mapped_memory) + endereco_real, dados, sizeof(T));
    }else{
        Endereco endereco_real = (endereco-ultimo_map) * tamanho_do_bloco;
        if (endereco_real > std::numeric_limits<off_t>::max()) {
            std::cerr << "Erro: overflow de endereço na escrita\n";
            return;
        }

        std::cout << endereco_real << std::endl;
        std::cout << ultimo_map << std::endl;
        memcpy(static_cast<char*>(mapped_memory) + endereco_real, dados, sizeof(T));
    }
}


#endif