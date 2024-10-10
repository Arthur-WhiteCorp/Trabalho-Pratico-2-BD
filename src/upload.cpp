#include <iostream>
#include <fstream>
#include "../include/OperationalSystemDescription.hpp"

int main(int argc, char* argv[]){
    OperationalSystemDescription os_info;
    std::vector<Device> infos;
    infos = os_info.getDevicesInformation();

    std::fstream my_csv;

    if (argc !=2){
        std::cout << "Número errado de argumentos, exemplo:"  << std::endl;
        std::cout << "upload <file.csv>" << std::endl;
        exit(1);
    }

    my_csv.open(argv[1],std::ios::in);

    if (!my_csv.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1; // Handle the error
    }

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

    

    

    return 0;
}