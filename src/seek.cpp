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
        Registro::VarChar
        };

    BlockManager block_manager = BlockManager(&data_base,registro);

    BlocoDeCatalogo* catalogo = static_cast<BlocoDeCatalogo*>(block_manager.LerBloco(0ull));

    std::cout << catalogo->endereco_do_arquivo_de_hash << std::endl;

    return 0;
}