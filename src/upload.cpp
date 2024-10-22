#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "OperationalSystemDescriptor.hpp"
#include "CSVReader.hpp"
#include "DiskManager.hpp"
#include "BlockManager.hpp"
#include "HashMaker.hpp"

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
        std::string name = info.device_name;
        unsigned long long device_size =  info.device_size;
        unsigned long sector_size = info.sector_size;
        unsigned long block_size = info.block_size;
        int sectors_per_block = info.sectors_per_block;

        std::cout << "Device Name: " << name << std::endl;
        std::cout << "Device Size: " << device_size << " bytes" << std::endl;
        std::cout << "Sector Size: " << sector_size << " bytes" << std::endl;
        std::cout << "Block Size: " << block_size << " bytes" << std::endl;
        std::cout << "Sectors per Block: " << sectors_per_block << std::endl;
        std::cout << "-----------------------" << std::endl; 
    }

    std::cout << std::endl;
    std::cout << std::endl;


    DiskManager banco_de_dados = DiskManager("./my_db.db",2048,&infos[0]);

    std::cout << data_base_csv.getTamanhoDoCSV() << " bytes de arquivo" << std::endl;


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


    BlocoDeArquivo bloco_de_arquivo;
    bloco_de_arquivo.tipo = Arquivo;
    bloco_de_arquivo.meta_dados = registro;
    bloco_de_arquivo.registro_a[0] = 'a';
    bloco_de_arquivo.registro_b[0] = 'b';
    bloco_de_arquivo.endereço_bucket_overflow = 0;

    BlockManager block_manager = BlockManager(&banco_de_dados,registro);

    unsigned long long endereco = banco_de_dados.memoryAlloc(1U);

    
    block_manager.EscreverBloco(&bloco_de_arquivo,endereco);
    
    BlocoDeArquivo* bloco_lido = static_cast<BlocoDeArquivo*>(block_manager.LerBloco(endereco));
    std::cout << bloco_lido->tipo << std::endl;
    std::cout << bloco_lido->registro_b[0] << std::endl;

    std::cout << "--------testes---------" << std::endl;    
    
    int a = 8796544;

    block_manager.EscreverCampo(bloco_lido,'a',1u,&a);
    std::string texto = "a vida é bela fhsiofhisfdhiosfdh";
    block_manager.EscreverCampo(bloco_lido,'a',2u,texto.c_str());


    int* recebido =  static_cast<int*> (block_manager.LerCampo(bloco_lido,'a',1u));
    char* texto_recebido = static_cast<char*>(block_manager.LerCampo(bloco_lido,'a',2U));

    std::cout << *recebido << std::endl;
    std::cout << texto_recebido << std::endl;
    return 0;
}