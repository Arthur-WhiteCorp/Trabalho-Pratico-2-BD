#include "OperationalSystemDescriptor.hpp"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>       
#include <unistd.h> 
#include <filesystem>
#include <vector>
#include <regex>
#include <tuple>

OperationalSystemDescriptor::OperationalSystemDescriptor(){
    devices_path = "/dev/";
    setDeviceNames();
    setDevicesInformation();
}
void OperationalSystemDescriptor::setDeviceNames(){
    const std::filesystem::path path = devices_path;
    std::regex device_regex("^(sd[a-z]\\d*|hd[a-z]\\d*|nvme\\d+n\\d+|mmcblk\\d+p?\\d*)$");
    std::vector<std::string> device_names;

    for(const auto& entry : std::filesystem::directory_iterator(path)){
        std::string device_name = entry.path().filename().string();
        if (std::regex_match(device_name, device_regex)){
            device_names.push_back(device_name);
        }
    }

    std::sort(device_names.begin(), device_names.end());

    for (const auto& name : device_names) {
        Device device;
        device.device_name = name;
        devices.push_back(device);
    };
}

void OperationalSystemDescriptor::setDevicesInformation(){
    for (auto& device: devices){
        std::string device_path = "/dev/" + device.device_name;
        int fd = open(device_path.c_str(), O_RDONLY);

        if (fd == -1) {
            std::cerr << "Error opening device" << std::endl;
        }

        setDeviceSize(fd,&device);
        setSectorSize(fd,&device);
        setBlockSize(fd,&device);
        setSectorPerBlock(&device);
        close(fd);
    }
}

void  OperationalSystemDescriptor::setDeviceSize(int fd, Device* device){
    unsigned long long device_size;

    if (ioctl(fd, BLKGETSIZE64, &device_size) == -1) {
        std::cerr << "Error getting device size" << std::endl;
    }
    device->device_size = device_size;

};

void  OperationalSystemDescriptor::setSectorSize(int fd, Device* device){
    unsigned int sector_size;

    if (ioctl(fd, BLKSSZGET, &sector_size) == -1) {
        std::cerr << "Error getting sector size" << std::endl;
    }
    device->sector_size = sector_size;

}
void  OperationalSystemDescriptor::setBlockSize(int fd, Device* device){
    unsigned int block_size;

    if (ioctl(fd, BLKBSZGET, &block_size) == -1) {
        std::cerr << "Error getting device size" << std::endl;
    }
    device->block_size = block_size;

}
void  OperationalSystemDescriptor::setSectorPerBlock(Device* device){
    int sectors_per_block = device->block_size / device->sector_size;
    device->sectors_per_block = sectors_per_block;

}


std::vector<std::string> OperationalSystemDescriptor::getDeviceNames(){
    std::vector<std::string> device_names;

    for(const auto& entry: devices){
        device_names.push_back(entry.device_name);
    }

    return device_names;
}

std::vector<unsigned long long int> OperationalSystemDescriptor::getDeviceSizes(){
    std::vector<unsigned long long int> device_sizes;

    for(const auto& device: devices){
        device_sizes.push_back(device.device_size);

    }

    return device_sizes;
}

std::vector<Device> OperationalSystemDescriptor::getDevicesInformation(){
    std::vector<Device> devices_information;
    for (const auto& device: devices){
        Device my_device;
        my_device.device_name  = device.device_name;
        my_device.device_size = device.device_size;
        my_device.sector_size = device.sector_size;
        my_device.block_size = device.block_size;
        my_device.sectors_per_block = device.sectors_per_block;
        
        devices_information.push_back(my_device);
    }

    return devices_information;

}

