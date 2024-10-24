#include "HashManager.hpp"
#include <cmath>
HashManager::HashManager(unsigned long long quantidade_de_linhas_do_arquivo, DiskManager* bando_de_dados
                        ,BlockManager* block_manager){
    this->bando_de_dados = bando_de_dados;
    this->block_manager = block_manager;
    setQuantidadeDeBlocosDeArquivo(quantidade_de_linhas_do_arquivo);
    setTamanhoDoHash();
    setQuantidadeDeBlocosDeHash();
    setTamanhoDoArquivoNoBancoDeDados();
    setHashTable();
}

void HashManager::setQuantidadeDeBlocosDeArquivo(unsigned long long quantidade_de_linhas_do_arquivo){
    unsigned long long tamanho = (quantidade_de_linhas_do_arquivo / REGISTROS_POR_BLOCO) + 1;
    quantidade_de_blocos_de_arquivo = tamanho;
    quantidade_de_blocos_enderecados_no_hash = tamanho + (tamanho/3) + 1; // adicona 30% a mais
    std::cout << "quantidade de blocos de arquivos: " << quantidade_de_blocos_de_arquivo << std::endl;
}

void HashManager::setTamanhoDoHash(){
    unsigned long long memoria = sizeof(Endereco) * quantidade_de_blocos_enderecados_no_hash;
    std::cout << "este é o tamanho do hash:" <<  memoria  << std::endl;
}

void HashManager::setQuantidadeDeBlocosDeHash(){
    quantidade_de_blocos_de_hash = (quantidade_de_blocos_enderecados_no_hash / ITEMS_DE_HASH_POR_BLOCO) + 1;
    std::cout << "quantidade de blocos de hash: " << quantidade_de_blocos_de_hash << std::endl;
}

void HashManager::setTamanhoDoArquivoNoBancoDeDados(){
    tamanho_do_arquivo_no_banco_de_dados = quantidade_de_blocos_enderecados_no_hash * bando_de_dados->getTamanhoDoBloco();
    std::cout << "tamanho do bloco de dados: " << bando_de_dados->getTamanhoDoBloco() << std::endl;
    std::cout << "tamanho do arquivo no banco de dados: " << tamanho_do_arquivo_no_banco_de_dados << std::endl;
}

void HashManager::setHashTable(){
    hash_table = new Endereco[quantidade_de_blocos_enderecados_no_hash];

    for (unsigned long long i = 0; i < quantidade_de_blocos_enderecados_no_hash; i++){
        hash_table[i] = bando_de_dados->memoryAlloc(1u);
    }
    
}

unsigned long long HashManager::hash(unsigned int id){
    return ((ALFA*id + BETA) % PRIMO_KNUTH) % quantidade_de_blocos_de_arquivo;

}

void HashManager::inserirNoHash(unsigned int id, void* dados){
    unsigned long long posicao = hash(id);
    block_manager->EscreverBloco(dados, hash_table[posicao]);
}

void* HashManager::buscarNoHash(unsigned int id){
    unsigned long long posicao = hash(id);
    return block_manager->LerBloco(hash_table[posicao]);
}