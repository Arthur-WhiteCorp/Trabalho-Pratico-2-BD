#ifndef OS_DESCRIPTION
#define OS_DESCRIPTION

#include <iostream>
#include <sys/ioctl.h>
#include <filesystem>
#include <vector>


class OperationalSystemDescription{
    private:
    struct Device{
        std::string device_name;
        unsigned long device_size;
        unsigned long  sector_size;
        unsigned long block_size;
        int sectors_per_block;
    };

    std::string devices_path;
    std::vector<Device> devices; //vetor com meus dispositivos 
    
    std::string getDevicesNames();
    unsigned long getDeviceSize();
    unsigned long getSectorSize();
    unsigned long getBlockSize();
    int getSectorPerBlock();

    public:
    OperationalSystemDescription();
      

};

#endif