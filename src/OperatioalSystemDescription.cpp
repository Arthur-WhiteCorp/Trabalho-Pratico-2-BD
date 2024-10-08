#include "../include/OperationalSystemDescription.hpp"
#include <iostream>
#include <sys/ioctl.h>
#include <filesystem>
#include <vector>


OperationalSystemDescription::OperationalSystemDescription(){
    devices_path = "/dev/";


}

std::string OperationalSystemDescription::getDevicesNames(){
    
}