#include <iostream>
#include "../include/OperationalSystemDescription.hpp"
#include <tuple>


int main(){
    OperationalSystemDescription os_info;
    std::vector<DeviceInfo> infos;
    infos = os_info.getDevicesInformation();

    for (const auto& info:infos){
        std::string name = std::get<0>(info);
        unsigned long long device_size =  std::get<1>(info);
        unsigned long sector_size = std::get<2>(info);
        unsigned long block_size = std::get<3>(info);
        int sectors_per_block = std::get<4>(info);

        std::cout << "Device Name: " << name << std::endl;
        std::cout << "Device Size: " << device_size << " bytes" << std::endl;
        std::cout << "Sector Size: " << sector_size << " bytes" << std::endl;
        std::cout << "Block Size: " << block_size << " bytes" << std::endl;
        std::cout << "Sectors per Block: " << sectors_per_block << std::endl;
        std::cout << "-----------------------" << std::endl; // Separator for better readability
    }

    

    return 0;
}