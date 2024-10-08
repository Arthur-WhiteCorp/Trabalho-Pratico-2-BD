#ifndef OS_DESCRIPTION
#define OS_DESCRIPTION

#include <iostream>
#include <sys/ioctl.h>
#include <filesystem>
#include <vector>
#include <tuple>

struct Device{
        std::string device_name;
        unsigned long long device_size;
        unsigned int sector_size;
        unsigned int block_size;
        int sectors_per_block;
};

class OperationalSystemDescription{
    private:
    
    std::string devices_path;
    std::vector<Device> devices; //vetor com meus dispositivos 
    
    void setDeviceNames();
    void setDeviceSize(int file_descriptor, Device *device);
    void setSectorSize(int file_descriptor, Device *device);
    void setBlockSize(int file_descriptor, Device *device);
    void setSectorPerBlock(Device *device);
    void setDevicesInformation();


    public:
    OperationalSystemDescription();
    std::vector<std::string> getDeviceNames();
    std::vector<unsigned long long> getDeviceSizes();
    std::vector<Device> getDevicesInformation();

};

#endif