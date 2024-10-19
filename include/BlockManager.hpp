#ifndef BLOCK_MANAGER
#define BLOCK_MANAGER

#include <array>
#include "DiskManager.hpp"
#include "HashMaker.hpp"


//Nenhum dos a segur define esses tamanhos, são somente marcações
#define TAMANHO_DO_REGISTRO 1503UL // o registro tem 1503 bytes
#define TAMANHO_DO_BLOCO_DE_ARQUIVO 3112UL // em bytes
#define TAMANHO_DO_BLOCO_DE_HASH 4088UL // em bytes
#define TAMANHO_DO_BLOCO_DE_INDICE  // em bytes
#define TAMANHO_DO_BLOCO_DE_INDICE_SECUNDARIO // em bytes

enum TipoDeBloco {Hash, Arquivo, Indice, IndiceSecundario};

#pragma pack(push, 1) 
struct Registro{ // 88 bytes
    enum Tipo {Int, VarChar, Data}; // INT = unsigned int; VARCHAR = char[n]; DATA = char[19] 
    unsigned int tamanho_do_registro = 0; // tamanho total em bytes do registro
    unsigned int quantidade_de_campos = 0; // máximo 10 campos
    std::array<unsigned int,10> tamanho_dos_campos{}; // inicia tudo com 0, é ordenado
    std::array<Tipo,10> tipos_dos_campos{}; // vetor ordenado de tipos de campo, inciado com tudo 0
};

struct BlocoDeArquivo{ // 3112 bytes
    TipoDeBloco tipo = Arquivo;
    Registro meta_dados;
    char registro_a[1503];
    char registro_b[1503];
    unsigned long long endereço_bucket_overflow; // endereço lógico do bucket de overflow

};

struct BlocoDeHash{ // 4088 bytes
    TipoDeBloco tipo = Hash; // tipo de bloco, 8 bytes
    std::array<ItemDoHash,255> items_do_hash; // items do hash, 4080 bytes

};

struct BlocoDeIndice{
    TipoDeBloco tipo = Indice;
};

struct BlocoDeIndiceSecundario{
    TipoDeBloco tipo = IndiceSecundario;
};
#pragma pack(pop) 

class BlockManager{
private:
    Registro tipo_de_registro; // cada bloco de Arquivo recebe  somente um tipo de registro


public:
    BlockManager(Registro registro);
    BlockManager();

    BlocoDeArquivo* LerBloco(unsigned long long endereco);
    void EscreverBloco(BlocoDeArquivo bloco, unsigned long long endereco, TipoDeBloco tipo);

    void* LerCampo(BlocoDeArquivo bloco, unsigned short int campo);
    void EscreverCampo(BlocoDeArquivo bloco,unsigned short int campo);



};

#endif