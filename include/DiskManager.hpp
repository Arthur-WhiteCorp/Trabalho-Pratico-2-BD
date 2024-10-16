#ifndef DISK_MANAGER
#define DISK_MANAGER

#include <vector>
#include <fcntl.h> 
#include <iostream>
#include "OperationalSystemDescriptor.hpp"

class DiskManager{
private:

    const char* file_path; // como estou usando baixo nível não posso usar string
    int arquivo;// file descriptor de baxio nível deferente do file stream
    unsigned int espaco_de_memoria; // em megabytes
    unsigned int tamanho_do_bloco; // em bytes
    unsigned int quantidade_de_blocos;
    unsigned long long prox_endereco_de_procura = 0; // endero onde a procura de espaco vai começar
    std::vector <bool> vetor_alocacao; // vetor de alocação dos blocos (true livre) (false alocado)
    std::vector <unsigned short int> vetor_espaco; // vetor de espaços de endereço de memoria

    int criarEspacoNaMemoria();
    void setVetorAlocacao();
    void setVetorEspaco();
    void setQuantidadeDeBlocos();

    void preencheEndereco(unsigned int endereço, unsigned int tamanho);
    unsigned long long procuraEndereco(unsigned int tamanho); // de "tamanho" blocos


public:
    
    DiskManager(const char* file_path, unsigned int espaco_de_memoria, Device* dispositivo_de_memoria);
    const std::vector<bool>& getVetorAlocacao() const;
    const std::vector<unsigned short int>& getVetorEspaco() const;
    unsigned long long getProxEnderecoDeProcura() const;
    unsigned long long memoryAlloc(unsigned int tamanho);
    void memoryDisalloc(unsigned long long endereco);
};

#endif