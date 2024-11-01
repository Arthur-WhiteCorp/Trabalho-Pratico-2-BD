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

int main(int argc, char* argv[]) {
    DiskManager data_base = DiskManager("./metadata.dsk","./my_db.db");
    if (argc != 2) {
        std::cerr << "seek <idnumber>" << std::endl;
        exit(1);
    }

    unsigned int id = std::stoi(argv[1]);

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
        Registro::VarChar
        };
    std::copy(temp_2.begin(), temp_2.end(), registro.tipos_dos_campos.begin()); 

    BlockManager block_manager = BlockManager(&data_base,registro);
    block_manager.carregarCatalogo(0ull);
    HashManager hash_manager = HashManager(&data_base,&block_manager);
    //7789
    Linha buscada_no_hash = hash_manager.buscarNoHash(id);
    Linha vazio = std::make_tuple(0, "", 0, "", 0, "", "");

    if (buscada_no_hash != vazio and id >= 0){
        std::cout << "id: " << std::get<0>(buscada_no_hash) << std::endl;
        std::cout << "titulo: " << std::get<1>(buscada_no_hash) << std::endl;
        std::cout << "ano: " << std::get<2>(buscada_no_hash) << std::endl;
        std::cout << "Autores: " << std::get<3>(buscada_no_hash) << std::endl;
        std::cout << "Citacoes: " << std::get<4>(buscada_no_hash) << std::endl;
        std::cout << "Atualizacao: " << std::get<5>(buscada_no_hash) << std::endl;
        std::cout << "Snippet: " << std::get<6>(buscada_no_hash) << std::endl;
    }else{
        std::cout << "Registro Nao Encontrado" << std::endl;
    }
    std::cout << "Quantidade de blocos lidos na busca: " <<
                  hash_manager.getQuatidadeDeBlocosLidos() << std::endl;
    std::cout << "Blocos de arquivo: " << block_manager.getQuantidadeDeBlocosEnderecadosNoHash() << std::endl;

    return 0;
}