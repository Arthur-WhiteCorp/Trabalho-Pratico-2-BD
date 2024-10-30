#include "HashManager.hpp"
#include <cmath>
HashManager::HashManager(unsigned long long quantidade_de_linhas_do_arquivo, DiskManager* banco_de_dados
                        ,BlockManager* block_manager){
    this->banco_de_dados = banco_de_dados;
    this->block_manager = block_manager;
    setQuantidadeDeBlocosDeArquivo(quantidade_de_linhas_do_arquivo);
    setTamanhoDoHash();
    setQuantidadeDeBlocosDeHash();
    setTamanhoDoArquivoNoBancoDeDados();
    setBlocoDeInicializacaoPadrao();
    setBucketOverflow();
    setHashTable();
}

void HashManager::setQuantidadeDeBlocosDeArquivo(unsigned long long quantidade_de_linhas_do_arquivo){
    unsigned long long tamanho = (quantidade_de_linhas_do_arquivo / REGISTROS_POR_BLOCO) + 1;
    quantidade_de_blocos_de_arquivo = tamanho;
    quantidade_de_blocos_enderecados_no_hash = tamanho + (tamanho/3) + 1; // adicona 30% a mais
    std::cout << "quantidade de blocos de arquivos: " << quantidade_de_blocos_de_arquivo << std::endl;
    std::cout << "quantidade de blocos enderecados no hash: " << quantidade_de_blocos_enderecados_no_hash << std::endl;
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
    tamanho_do_arquivo_no_banco_de_dados = quantidade_de_blocos_enderecados_no_hash * banco_de_dados->getTamanhoDoBloco();
    std::cout << "tamanho do bloco de dados: " << banco_de_dados->getTamanhoDoBloco() << std::endl;
    std::cout << "tamanho do arquivo no banco de dados: " << tamanho_do_arquivo_no_banco_de_dados << std::endl;
}

void HashManager::setHashTable(){
    hash_table = new Endereco[quantidade_de_blocos_enderecados_no_hash];
    for (unsigned long long i = 0; i < quantidade_de_blocos_enderecados_no_hash; i++){
        hash_table[i] = banco_de_dados->memoryAlloc(1u);
        block_manager->EscreverBloco(&bloco_de_inicialização_padrao, hash_table[i]);
       
    }
    banco_de_dados->sincronizar();

}

unsigned long long HashManager::hash(unsigned int id){
    return ((ALFA*id + BETA) % PRIMO_KNUTH) % quantidade_de_blocos_de_arquivo;

}

void HashManager::inserirNoHash(unsigned int id, Linha dados){

    unsigned long long posicao = hash(id);
    char escrever_em;
    std::string titulo = std::get<1>(dados);
    unsigned short int ano = std::get<2>(dados);
    std::string autores = std::get<3>(dados);
    unsigned int citacoes = std::get<4>(dados);
    std::string atualizacao = std::get<5>(dados);
    std::string snippet = std::get<6>(dados);

    BlocoDeArquivo* bloco = static_cast<BlocoDeArquivo*>(block_manager->LerBloco(hash_table[posicao]));
    if (bloco->livre[0]){
        escrever_em = 'a';
        bloco->livre[0] = false;
    }else if (bloco->livre[1]){
        escrever_em = 'b';
        bloco->livre[1] = false; 
    }else{
        escrever_em = 'o'; // escrever no bcuket de overflow
    }

    if (escrever_em == 'a' or escrever_em == 'b'){
        
        block_manager->EscreverCampo(bloco, escrever_em, 1, &id);
        block_manager->EscreverCampo(bloco, escrever_em, 2,titulo.c_str());    
        block_manager->EscreverCampo(bloco,escrever_em,3,&ano);
        block_manager->EscreverCampo(bloco,escrever_em,4,autores.c_str());
        block_manager->EscreverCampo(bloco,escrever_em,5,&citacoes);
        block_manager->EscreverCampo(bloco,escrever_em,6,atualizacao.c_str());
        block_manager->EscreverCampo(bloco,escrever_em,7,snippet.c_str());
        block_manager->EscreverBloco(bloco,hash_table[posicao]);
        return;
    }else{

        insereOverflow(dados,bloco->endereço_bucket_overflow);

    }
}
void HashManager::insereOverflow(Linha dados, Endereco overflow){


    char escrever_em;
    unsigned int id = std::get<0>(dados);
    std::string titulo = std::get<1>(dados);
    unsigned short int ano = std::get<2>(dados);
    std::string autores = std::get<3>(dados);
    unsigned int citacoes = std::get<4>(dados);
    std::string atualizacao = std::get<5>(dados);
    std::string snippet = std::get<6>(dados);
    BlocoDeArquivo* bloco = static_cast<BlocoDeArquivo*>(block_manager->LerBloco(overflow));

    if (bloco->endereço_bucket_overflow == 0ull){
        bloco->endereço_bucket_overflow = banco_de_dados->memoryAlloc(1u);
        BlocoDeArquivo overflow = bloco_de_inicialização_padrao;
        overflow.endereço_bucket_overflow = 0;
        block_manager->EscreverBloco(&overflow,bloco->endereço_bucket_overflow);
    }



    if (bloco->livre[0]){
        escrever_em = 'a';
        bloco->livre[0] = false;
    }else if (bloco->livre[1]){
        escrever_em = 'b';
        bloco->livre[1] = false; 
    }else{
        escrever_em = 'o'; // escrever no bcuket de overflow
    }

    if (escrever_em == 'a' or escrever_em == 'b'){
        block_manager->EscreverCampo(bloco, escrever_em, 1, &id);
        block_manager->EscreverCampo(bloco, escrever_em, 2,titulo.c_str());    
        block_manager->EscreverCampo(bloco,escrever_em,3,&ano);
        block_manager->EscreverCampo(bloco,escrever_em,4,autores.c_str());
        block_manager->EscreverCampo(bloco,escrever_em,5,&citacoes);
        block_manager->EscreverCampo(bloco,escrever_em,6,atualizacao.c_str());
        block_manager->EscreverCampo(bloco,escrever_em,7,snippet.c_str());
        block_manager->EscreverBloco(bloco,overflow);
        return;
    }else{

        insereOverflow(dados,bloco->endereço_bucket_overflow);

    }


}


Linha HashManager::buscarNoHash(unsigned int id){
    unsigned long long posicao = hash(id);
    bool achado = false;
    char localizacao;
    BlocoDeArquivo* bloco = static_cast<BlocoDeArquivo*>(block_manager->LerBloco(hash_table[posicao]));
    Linha registro;
    unsigned int id_registro_a = *static_cast<unsigned int*>(block_manager->LerCampo(bloco,'a',1u));
    unsigned int id_registro_b = *static_cast<unsigned int*>(block_manager->LerCampo(bloco,'b',1u));

    if (id == id_registro_a){
        achado = true;
        localizacao = 'a';

    }else if (id == id_registro_b){
        achado = true;
        localizacao = 'b';
    }

    if ( localizacao == 'a' or localizacao == 'b'){
        std::string titulo = static_cast<const char*> (block_manager->LerCampo(bloco,localizacao,2u));
        unsigned short ano = *static_cast<unsigned short int*> (block_manager->LerCampo(bloco,localizacao,3u));
        std::string autores = static_cast<const char*>(block_manager->LerCampo(bloco,localizacao,4u));
        unsigned int citacoes = *static_cast<int*>(block_manager->LerCampo(bloco,localizacao,5u));
        std::string atualizacoes = static_cast<const char*> (block_manager->LerCampo(bloco,localizacao,6u));
        std::string snippet = static_cast<const char*> (block_manager->LerCampo(bloco,localizacao,7u));

        registro = std::make_tuple(
        id, 
        titulo, 
        ano, 
        autores, 
        citacoes, 
        atualizacoes, 
        snippet);
    }else if (!achado){
        return buscaOverflow(id,bloco->endereço_bucket_overflow);
    }

    return registro;

}

Linha HashManager::buscaOverflow(unsigned int id, Endereco overflow){

    bool achado = false;
    char localizacao;
    BlocoDeArquivo* bloco = static_cast<BlocoDeArquivo*>(block_manager->LerBloco(overflow));

    unsigned int id_registro_a = *static_cast<int*>(block_manager->LerCampo(bloco,'a',1u));
    unsigned int id_registro_b = *static_cast<int*>(block_manager->LerCampo(bloco,'b',1u));


    if (id == id_registro_a){
        achado = true;
        localizacao = 'a';

    }else if (id == id_registro_b){
        achado = true;
        localizacao = 'b';
    }

    if ( localizacao == 'a' or localizacao == 'b'){
        std::string titulo = static_cast<const char*> (block_manager->LerCampo(bloco,localizacao,2u));
        unsigned short ano = *static_cast<unsigned short int*> (block_manager->LerCampo(bloco,localizacao,3u));
        std::string autores = static_cast<const char*>(block_manager->LerCampo(bloco,localizacao,4u));
        unsigned int citacoes = *static_cast<int*>(block_manager->LerCampo(bloco,localizacao,5u));
        std::string atualizacoes = static_cast<const char*> (block_manager->LerCampo(bloco,localizacao,6u));
        std::string snippet = static_cast<const char*> (block_manager->LerCampo(bloco,localizacao,7u));

        Linha Registro = std::make_tuple(
        id, 
        titulo, 
        ano, 
        autores, 
        citacoes, 
        atualizacoes, 
        snippet);
        return Registro;
    }else if (!achado and bloco->endereço_bucket_overflow != 0){
        return buscaOverflow(id,bloco->endereço_bucket_overflow);

    }else{
        return {};
    }


}

void HashManager::setBlocoDeInicializacaoPadrao(){
    bloco_de_inicialização_padrao.endereço_bucket_overflow = banco_de_dados->memoryAlloc(1u);
    bloco_de_inicialização_padrao.meta_dados = block_manager->getTIpoDeRegistro();
    bloco_de_inicialização_padrao.tipo = Arquivo;
}

void HashManager::setBucketOverflow(){
    BlocoDeArquivo overflow = bloco_de_inicialização_padrao;
    overflow.endereço_bucket_overflow = 0;
    block_manager->EscreverBloco(&overflow, bloco_de_inicialização_padrao.endereço_bucket_overflow);

}

void HashManager::saveHash(){
    BlocoDeHash bloco_de_hash;
    Endereco endereco_de_insercao;
    int posicao_atual = 0;

    for (int i = 0; i < quantidade_de_blocos_enderecados_no_hash; i++){
        bloco_de_hash.items_do_hash[posicao_atual] = hash_table[i];
        posicao_atual++;
        if (posicao_atual == ITEMS_DE_HASH_POR_BLOCO){
            endereco_de_insercao = banco_de_dados->memoryAlloc(1u);
            block_manager->EscreverBloco(&bloco_de_hash,endereco_de_insercao);
            posicao_atual = 0;
        }
    }

    if (posicao_atual > 0) {
        // Zero out unused slots to avoid "dirty" data
        for (int i = posicao_atual; i < ITEMS_DE_HASH_POR_BLOCO; i++) {
            bloco_de_hash.items_do_hash[i] = 0;
        }
        endereco_de_insercao = banco_de_dados->memoryAlloc(1u);
        block_manager->EscreverBloco(&bloco_de_hash, endereco_de_insercao);
    }
}