#include "my_robot_hardware/driver.hpp"
#include "my_robot_hardware/motor.hpp"
#include <iostream>
namespace Robot{
    Driver::Driver(std::string device_name):Device(device_name){
        std::cout<<"Driver has open device successfully"<<"\n";
    }
   
    ::Robot::Motor Driver::add_motor(std::string motor_name,bool PWM_status, int IN_pin1, int IN_pin2,int Enable_pin){
        try{
           return ::Robot::Motor(*(this),motor_name,PWM_status,IN_pin1,IN_pin2,Enable_pin);
        }
        catch(std::exception& e){
            std::cout<<"motor not created"<<"\n";
            ::Robot::Motor mymotor;
            return mymotor;
        }

    }
}