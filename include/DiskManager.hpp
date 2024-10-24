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

    int criarEspacoNaMemoria();
    void setVetorAlocacao();
    void setVetorEspaco();
    void setQuantidadeDeBlocos();
    void preencheEndereco(Endereco endereco, unsigned int tamanho);
    Endereco procuraEndereco(unsigned int tamanho); // de "tamanho" blocos


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
    void* read(Endereco endereco) const;


};

template <typename T>

void DiskManager::write(Endereco endereco, T* dados){
    if (vetor_alocacao[endereco]){
        std::cerr << "Erro: endereço não alocado\n";
        std::cout << vetor_alocacao[endereco] << std::endl;
        return; // não escreve em endereços não alocados
    }


    Endereco endereco_real = endereco * tamanho_do_bloco;
    if (endereco_real > std::numeric_limits<off_t>::max()) {
        std::cerr << "Erro: overflow de endereço\n";
        return;
    }

    // Map the block into memory
    void* mapped_mem = mmap(NULL, tamanho_do_bloco, PROT_READ | PROT_WRITE,
                            MAP_SHARED, arquivo, static_cast<off_t>(endereco_real));
    if (mapped_mem == MAP_FAILED) {
        std::cerr << "mmap falhou.\n";
        return;
    }

    memcpy(mapped_mem, dados, sizeof(T));  // Ensure T fits within the block size
     if (msync(mapped_mem, tamanho_do_bloco, MS_SYNC) == -1) {
        std::cerr << "Erro: msync falhou: " << strerror(errno) << std::endl;
    }

    // Unmap the memory after use
    if (munmap(mapped_mem, tamanho_do_bloco) == -1) {
        std::cerr << "Erro: munmap falhou: " << strerror(errno) << std::endl;
    }

}


#endif