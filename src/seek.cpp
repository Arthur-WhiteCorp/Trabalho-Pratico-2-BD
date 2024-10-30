#include <iostream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include "OperationalSystemDescriptor.hpp"
#include "CSVReader.hpp"
#include "DiskManager.hpp"
#include "BlockManager.hpp"
#include "HashManager.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    DiskManager data_base = DiskManager("./metadata.dsk","./my_db.db");


    Registro registro;
    registro.tamanho_do_registro = TAMANHO_DO_REGISTRO;
    registro.quantidade_de_campos = 7;

    std::array<unsigned int, 7> temp = {4U, 300U, 2U, 150U, 4U, 19U, 1024U};
    std::copy(temp.begin(), temp.end(), registro.tamanho_dos_campos.begin()); 

    std::array<Registro::Tipo, 7> temp_2 = {
        Registro::Int, 
        Registro::Char, 
        Registro::Int, 
        Registro::Char, 
        Registro::Int, 
        Registro::Data, 
        Registro::VarChar};

    BlocoDeArquivo bloco_de_arquivo;
    bloco_de_arquivo.tipo = Arquivo;
    bloco_de_arquivo.meta_dados = registro;
    bloco_de_arquivo.registro_a[0] = 'a';
    bloco_de_arquivo.registro_b[0] = 'b';
    bloco_de_arquivo.endereço_bucket_overflow = 0;

    BlockManager block_manager = BlockManager(&data_base,registro);

    BlocoDeArquivo* bloco_lido = static_cast<BlocoDeArquivo*>(block_manager.LerBloco(0ull));

    

    int* recebido =  static_cast<int*> (block_manager.LerCampo(bloco_lido,'a',1u));
    char* texto_recebido = static_cast<char*>(block_manager.LerCampo(bloco_lido,'a',2U));
    unsigned short int* ano_recebido = static_cast<unsigned short int*>(block_manager.LerCampo(bloco_lido,'a',3U));


    std::cout << *recebido << std::endl;
    std::cout << texto_recebido << std::endl;
    std::cout << *ano_recebido << std::endl;

    return 0;
}