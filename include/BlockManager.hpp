#ifndef BLOCK_MANAGER
#define BLOCK_MANAGER

#include <array>
#include "DiskManager.hpp"


//Nenhum dos a seguir define esses tamanhos, são somente marcações
#define TAMANHO_DO_REGISTRO 1505UL // o registro tem 1505 bytes
#define TAMANHO_DO_BLOCO_DE_ARQUIVO 3114UL // em bytes
#define REGISTROS_POR_BLOCO 2U // 2 registros por bloco
#define TAMANHO_DO_BLOCO_DE_HASH 4092UL // em bytes
#define ITEMS_DE_HASH_POR_BLOCO 511UL // 511 items de hash por bloco
#define TAMANHO_DO_BLOCO_DE_INDICE  // em bytes
#define TAMANHO_DO_BLOCO_DE_INDICE_SECUNDARIO // em bytes


enum TipoDeBloco {Hash, Arquivo, Indice, IndiceSecundario, Catalogo, Erro}; // o tipo é 4 bytes

struct ItemDoHash;


#pragma pack(push, 1) 
struct Registro{ // 88 bytes
    enum Tipo {Int, Char, VarChar, Data}; // INT = unsigned int; VARCHAR = char[n]; DATA = char[19] // 4 bytes
    unsigned int tamanho_do_registro = 0; // tamanho total em bytes do registro
    unsigned int quantidade_de_campos = 0; // máximo 10 campos
    std::array<unsigned int,10> tamanho_dos_campos{}; // inicia tudo com 0, é ordenado
    std::array<Tipo,10> tipos_dos_campos{}; // vetor ordenado de tipos de campo, inciado com tudo 0
};

struct BlocoDeArquivo{ // 3114 bytes
    TipoDeBloco tipo = Arquivo;
    Registro meta_dados;
    std::array<bool,REGISTROS_POR_BLOCO> livre{true,true};
    std::array<unsigned char,TAMANHO_DO_REGISTRO> registro_a{};
    std::array<unsigned char,TAMANHO_DO_REGISTRO> registro_b{};
    Endereco endereço_bucket_overflow; // endereço lógico do bucket de overflow

};

struct BlocoDeCatalogo{ // 3114 bytes
    TipoDeBloco tipo = Catalogo;
    Endereco endereco_do_arquivo_de_hash;
    Endereco endereco_da_arvore_b_plus_primaria;
    Endereco endereco_da_arvore_b_plus_secundaria;

};

struct BlocoDeHash{ // 4092 bytes
    TipoDeBloco tipo = Hash; // tipo de bloco, 4 bytes
    std::array<Endereco,ITEMS_DE_HASH_POR_BLOCO> items_do_hash; // items do hash, 4080 bytes

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
    DiskManager* banco_de_dados;

public:
    BlockManager(DiskManager* banco_de_dados,  Registro registro);
    BlockManager(DiskManager* banco_de_dados);

    void setTipoDeRegistro(Registro registro);

    Registro getTIpoDeRegistro();

    TipoDeBloco getTipoDeBloco(Endereco endereco);

    void* LerBloco(Endereco endereco);
    void EscreverBloco(void* bloco, Endereco endereco);


    void* LerCampo(BlocoDeArquivo* bloco,char registro, unsigned short int campo); // registro a ou b
    void EscreverCampo(BlocoDeArquivo* bloco,char registro,unsigned short int campo, const void* data);

};

#endif