#ifndef HASH_MAKER
#define HASH_MAKER

#include <vector>
#include "BlockManager.hpp"

#define PRIMO_KNUTH 2654435761u
#define ALFA 654656465U
#define BETA 1054628546U

class HashManager{
private:
    DiskManager* bando_de_dados;
    BlockManager* block_manager;

    unsigned long long quantidade_de_blocos_de_hash;
    unsigned long long quantidade_de_blocos_de_arquivo;
    unsigned long long quantidade_de_blocos_enderecados_no_hash; // 30% a mais que a quantidade de blocos de arquivo
    unsigned long long tamanho_do_hash; // tamanho bruto em bytes sem overhead de meta-dados
    unsigned long long tamanho_do_arquivo_no_banco_de_dados;
    Endereco* hash_table;

    unsigned long long hash(unsigned int id); // estou usando hash multiplicativo
    void setQuantidadeDeBlocosDeArquivo(unsigned long long quantidade_de_linhas_do_arquivo);
    void setQuantidadeDeBlocosDeHash();
    void setTamanhoDoHash();
    void setTamanhoDoArquivoNoBancoDeDados();
    void setHashTable();
public:
    HashManager(unsigned long long quantidade_de_linhas_do_arquivo, DiskManager* bando_de_dados
                ,BlockManager* block_manager); 
    void inserirNoHash(unsigned int id, void* dados); // insere no hash
    void* buscarNoHash(unsigned int id); // busca no hash
};



#endif