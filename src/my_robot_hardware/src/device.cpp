#include "my_robot_hardware/device.hpp"
#include <iostream>

namespace Robot{

    Device::Device(std::string device_name){
        try
        {
            this->chip_ = ::gpiod::chip(device_name);
        }
        catch(const std::exception& e)
        {
            ::std::cerr << "Error opening GPIO chip: "<<e.what()<< '\n';
        }
        

    }

    Device ::~Device(){
        std::cout<<"Driver has closed device successfully"<<"\n";
    }
    // return chip_name
    

    
}