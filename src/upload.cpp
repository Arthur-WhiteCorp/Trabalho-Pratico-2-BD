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

 std::string retiraAspasDoCampo(std::vector<std::string> linha) {

        std::string result;
        std::string campo = linha[1];

        // Remove aspas do inicio e fim do campo titulo
        if ((campo.length() > 1) or (campo != "NULL")) {
            campo.erase(0, 1);
            campo.erase(campo.length()-1, 1);
        }

        if (campo.length() == 0 or campo == "NULL"){
            campo = "NULL";
        }

        result = campo;

        return result; 
}

int main(int argc, char* argv[]){
    OperationalSystemDescriptor os_info;
    std::vector<Device> infos;
    infos = os_info.getDevicesInformation();
    std::vector<std::vector<std::string>> linhas_do_csv;
    std::vector<std::string> nova_linha;

    if (argc !=2){
        std::cout << "Número errado de argumentos, exemplo:"  << std::endl;
        std::cout << "upload <file.csv>" << std::endl;
        exit(1);
    }

    CSVReader data_base_csv = CSVReader(argv[1]);
    data_base_csv.setNumeroDeColunas(7);

    for (const auto& info:infos){

        std::cout << "Device Name: " << info.device_name << std::endl;
        std::cout << "Device Size: " << info.device_size << " bytes" << std::endl;
        std::cout << "Sector Size: " << info.sector_size << " bytes" << std::endl;
        std::cout << "Block Size: " << info.block_size << " bytes" << std::endl;
        std::cout << "Sectors per Block: " << info.sectors_per_block << std::endl;
        std::cout << "-----------------------" << std::endl; 
    }

    


    DiskManager banco_de_dados = DiskManager("./my_db.db",3000u,&infos[0]);

    std::cout << data_base_csv.getTamanhoDoCSV() << " bytes de arquivo" << std::endl;

    std::cout << "--------testes---------" << std::endl;    

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

    std::copy(temp_2.begin(), temp_2.end(), registro.tipos_dos_campos.begin()); 


    BlockManager block_manager = BlockManager(&banco_de_dados,registro);

    unsigned long long endereco = banco_de_dados.memoryAlloc(1U);
    block_manager.setCatalogo(endereco);
    block_manager.carregarCatalogo();


    std::cout << "-------criando hash------" << std::endl;
  
    while (!(data_base_csv.getLineCSV().empty())){
        unsigned int id = (unsigned int)data_base_csv.getIdDaLinhaAtual();
        if (!data_base_csv.getLinhaAtual().empty()) {
            std::string titulo = retiraAspasDoCampo(data_base_csv.getLinhaAtual());
        }
    }
    
    unsigned long long quantidade_de_linhas = data_base_csv.getNumeroDeLinhasLido(); 
    std::cout << "quantidade de linhas lidas: " << quantidade_de_linhas << std::endl;
    data_base_csv.resetarLocalizacaoDoarquivo();
    
    //1021439ull

    HashManager arquivo_hash = HashManager(1021439ll,&banco_de_dados,&block_manager);

    Linha linha_teste = std::make_tuple(
        7789u, 
        "teste string 1", 
        2184u, 
        "teste string 2", 
        8u, 
        "teste string 3", 
        "teste 4"
    );
    arquivo_hash.inserirNoHash(7789u, linha_teste);



    Linha buscada_no_hash = arquivo_hash.buscarNoHash(7789u);

    std::cout << std::get<6>(buscada_no_hash) << std::endl;

    
    arquivo_hash.saveHash();
    banco_de_dados.saveDiskMetaData("./metadata.dsk");

    return 0;
}

