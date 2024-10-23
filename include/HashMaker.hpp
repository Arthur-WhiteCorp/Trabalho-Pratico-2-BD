#ifndef HASH_MAKER
#define HASH_MAKER

#include <vector>
#include "BlockManager.hpp"

using endereco = unsigned long long;


class HashMaker{
private:
    unsigned long quantidade_de_blocos_de_hash;
    unsigned long quantidade_de_blocos_de_arquivo;
    endereco* hash;

    unsigned long long fazHashing(unsigned int id);
    void setQuantidadeDeBlocosDeArquivo(uintmax_t tamanho_do_arquivo);
    void setTamanhoDoHash();
public:
    HashMaker(uintmax_t tamanho_do_arquivo); // em bytes
    int inserirNoHash(); // insere no hash
    void* buscarNoHash(unsigned int id); // busca no hash
};



#endif