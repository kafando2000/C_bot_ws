
#include "my_robot_hardware/motor.hpp"
// inspired from driver L298N
#ifndef ROBOT_DRIVER
#define ROBOT_DRIVER

namespace Robot{
    class Driver: public Device{
        public:
            Driver(std::string device_name);
            ::Robot::Motor add_motor( std::string motor_name,bool PWM_status, int IN_pin1, int IN_pin2,int Enable_pin);

        private:


    };



}

#endif