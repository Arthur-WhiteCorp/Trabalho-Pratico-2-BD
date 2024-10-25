#include <iostream>
#include "DiskManager.hpp"
#include "BlockManager.hpp"

BlockManager::BlockManager(DiskManager* banco_de_dados){
    this->banco_de_dados = banco_de_dados;
}

BlockManager::BlockManager(DiskManager* banco_de_dados ,Registro registro){
    this->banco_de_dados = banco_de_dados;

    tipo_de_registro.tamanho_do_registro = registro.tamanho_do_registro;
    tipo_de_registro.quantidade_de_campos = registro.quantidade_de_campos;
    tipo_de_registro.tamanho_dos_campos = registro.tamanho_dos_campos;
    tipo_de_registro.tipos_dos_campos = registro.tipos_dos_campos;
}

void BlockManager::setTipoDeRegistro(Registro registro){
    tipo_de_registro.tamanho_do_registro = registro.tamanho_do_registro;
    tipo_de_registro.quantidade_de_campos = registro.quantidade_de_campos;
    tipo_de_registro.tamanho_dos_campos = registro.tamanho_dos_campos;
    tipo_de_registro.tipos_dos_campos = registro.tipos_dos_campos;
}

TipoDeBloco BlockManager::getTipoDeBloco(Endereco endereco){
    void* data = banco_de_dados->read(endereco);
    unsigned char* byte_data = static_cast<unsigned char*>(data);
    TipoDeBloco tipo_de_bloco = static_cast<TipoDeBloco>(*byte_data);
    
    if (tipo_de_bloco >=0 and tipo_de_bloco <= 3){
        return tipo_de_bloco;
    }

    return Erro;

}

Registro BlockManager::getTIpoDeRegistro(){
    return tipo_de_registro;
}

void* BlockManager::LerBloco(Endereco endereco) {
    void* data = banco_de_dados->read(endereco);
    return data;
}


void BlockManager::EscreverBloco(void* bloco, Endereco endereco){

    unsigned char* byte_data = static_cast<unsigned char*>(bloco);
    TipoDeBloco tipo_de_bloco = static_cast<TipoDeBloco>(*byte_data);

     
    switch (tipo_de_bloco){
    case Arquivo: {
        BlocoDeArquivo* bloco_arquivo = static_cast<BlocoDeArquivo*>(bloco);
        banco_de_dados->write(endereco, bloco_arquivo);
        break;
    }
    case Hash: {
        BlocoDeHash* bloco_hash = static_cast<BlocoDeHash*>(bloco);
        banco_de_dados->write(endereco, bloco_hash);
        break;
    }
    case Indice: {
        BlocoDeIndice* bloco_indice = static_cast<BlocoDeIndice*>(bloco);
        banco_de_dados->write(endereco, bloco_indice);  
        break;
    }
    case IndiceSecundario:{
        BlocoDeIndiceSecundario* bloco_indice_secundario = static_cast<BlocoDeIndiceSecundario*>(bloco);
        banco_de_dados->write(endereco, bloco_indice_secundario);
        break;
    }
    }   
}

void* BlockManager::LerCampo(BlocoDeArquivo* bloco, char registro ,unsigned short int campo){
    unsigned long offset_de_registro = 0;
    unsigned long offset_de_campo = 0;
    unsigned long delimitador_de_campo = 0;

    switch (registro){
    case 'a':
        offset_de_registro = 94UL; // 92 bytes 4 do tipo + 2 do vetor livre+ 88 dos metadados 
        break;
    case 'b':
        offset_de_registro = 94UL + TAMANHO_DO_REGISTRO; // pula o primeiro registro
        break;
    default:
        return nullptr;
        std::cerr << "Erro: registro inválido" << std::endl;
        break;
    }

    unsigned char* byte_data = reinterpret_cast<unsigned char*>(bloco);

  

    for (int i = 0; i < campo - 1; i++){
        offset_de_campo += bloco->meta_dados.tamanho_dos_campos[i];
    }

    delimitador_de_campo = offset_de_campo + 
                        bloco->meta_dados.tamanho_dos_campos[campo - 1] +
                        offset_de_registro;
    size_t campo_size = bloco->meta_dados.tamanho_dos_campos[campo - 1];

    
    unsigned char* campo_data = (unsigned char*) malloc(sizeof(unsigned char) * campo_size);
    memcpy(campo_data, byte_data + offset_de_campo + offset_de_registro, campo_size);
    
    return campo_data;
}

void BlockManager::EscreverCampo(BlocoDeArquivo* bloco,char registro,unsigned short int campo, const void* data){
    unsigned long offset_de_registro = 0;
    unsigned long offset_de_campo = 0;
    unsigned long delimitador_de_campo = 0;

    switch (registro){
    case 'a':
        offset_de_registro = 94UL; // 92 bytes 4 do tipo + 2 do vetor livre+ 88 dos metadados 
        break;
    case 'b':
        offset_de_registro = 94UL + TAMANHO_DO_REGISTRO; // pula o primeiro registro
        break;
    default:
        std::cerr << "Erro: registro inválido" << std::endl;
        break;
    }

    unsigned char* byte_data = reinterpret_cast<unsigned char*>(bloco);

    for (int i = 0; i < campo - 1; i++){
        offset_de_campo += bloco->meta_dados.tamanho_dos_campos[i];
    }

    delimitador_de_campo = offset_de_campo + 
                        bloco->meta_dados.tamanho_dos_campos[campo - 1] +
                        offset_de_registro;
    size_t campo_size = bloco->meta_dados.tamanho_dos_campos[campo - 1];


    memcpy(byte_data+offset_de_campo+offset_de_registro,data,campo_size);


}

