#ifndef DISK_MANAGER
#define DISK_MANAGER

#include <vector>
#include <fcntl.h> 
#include <iostream>
#include "OperationalSystemDescription.hpp"

class DiskManager{
private:
    const char* file_path; // como estou usando baixo nível não posso usar string
    int arquivo; // file descriptor de baxio nível deferente do file stream
    unsigned int espaco_de_memoria; // em megabytes
    unsigned int tamanho_do_bloco; // em bytes
    unsigned int quantidade_de_blocos;
    std::vector <bool> vetor_alocacao; // vetor de alocação dos blocos (true livre) (false alocado)
    int criarEspacoNaMemoria();
    void setVetorAlocacao();
    void setQuantidadeDeBlocos();

public:
    unsigned long long memoryAlloc();
    DiskManager(const char* file_path, unsigned int espaco_de_memoria, Device* dispositivo_de_memoria);
};

#endif