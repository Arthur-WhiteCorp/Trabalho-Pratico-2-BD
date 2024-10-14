#ifndef DISK_MANAGER
#define DISK_MANAGER

#include <vector>

class DiskManager{
private:
    unsigned int espaco_de_memoria; // em megabytes
    std::vector <char> vetor_alocacao; // vetor de alocação dos blocos
    void setVetorAlocacao();

public:
    void setEspacoDeMemoria(unsigned int);
    unsigned long long memoryAlloc();


    DiskManager(/* args */);
};

#endif