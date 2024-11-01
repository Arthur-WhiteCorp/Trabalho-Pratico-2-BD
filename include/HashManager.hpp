#ifndef HASH_MAKER
#define HASH_MAKER

#include <vector>
#include "BlockManager.hpp"

#define PRIMO_KNUTH 2654435761u
#define ALFA 654656465U
#define BETA 1054628546U


using Linha = std::tuple<unsigned int, std::string, unsigned short int, std::string, unsigned int , std::string,
                         std::string>;
class HashManager{
private:
    DiskManager* banco_de_dados;
    BlockManager* block_manager;

    unsigned long long quantidade_de_blocos_de_hash;
    unsigned long long quantidade_de_blocos_de_arquivo;
    unsigned long long quantidade_de_blocos_enderecados_no_hash; // 30% a mais que a quantidade de blocos de arquivo
    unsigned long long tamanho_do_hash; // tamanho bruto em bytes sem overhead de meta-dados
    unsigned long long tamanho_do_arquivo_no_banco_de_dados;
    Endereco* hash_table;
    BlocoDeArquivo bloco_de_inicialização_padrao;
    Endereco endereco_do_ultimo_bloco_de_overflow;
    unsigned int quantidade_de_bloco_lidos_na_ultima_busca = 0;

    unsigned long long hash(unsigned int id); // estou usando hash multiplicativo
    void setQuantidadeDeBlocosDeArquivo(unsigned long long quantidade_de_linhas_do_arquivo);
    void setQuantidadeDeBlocosDeHash();
    void setTamanhoDoHash();
    void setTamanhoDoArquivoNoBancoDeDados();
    void setBlocoDeInicializacaoPadrao();
    void setHashTable();
    void setBucketOverflow();

    void insereOverflow(Linha dados, Endereco overflow); // insere no bucket de overflow
    Linha buscaOverflow(unsigned int id, Endereco overflow);
public:
    HashManager(unsigned long long quantidade_de_linhas_do_arquivo, DiskManager* banco_de_dados
                ,BlockManager* block_manager);
    HashManager(DiskManager* banco_de_dados ,BlockManager* block_manager);
    void inserirNoHash(unsigned int id, Linha dados); // insere no hash
    Linha buscarNoHash(unsigned int id); // busca no hash
    unsigned int getQuatidadeDeBlocosLidos(); // ultima quantidade de blocos lidos na ultima busca

    void saveHash();
    void loadHash(Endereco endereco);


};



#endif