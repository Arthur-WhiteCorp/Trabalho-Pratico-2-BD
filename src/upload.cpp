#include <iostream>
#include <fstream>
#include "OperationalSystemDescription.hpp"
#include "CSVReader.hpp"

int main(int argc, char* argv[]){
    OperationalSystemDescription os_info;
    std::vector<Device> infos;
    infos = os_info.getDevicesInformation();

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

    while (!data_base_csv.getArquivoTerminado())
    {
        for (const auto& campo:data_base_csv.getLineCSV()){
            std::cout << campo << std::endl;
        }
    }
    
    std::cout << "arquivo lido com sucesso" << std::endl;
    

    return 0;
}