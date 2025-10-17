#include <gpiod.hpp>
#include "my_robot_hardware/device.hpp"
#ifndef ROBOT_MOTOR__HPP
#define ROBOT_MOTOR__HPP

namespace Robot{
    class Driver;
}
namespace Robot{
   class Motor{
     public:
         Motor();
         Motor(::Robot::Driver& motordriver,std::string motor_name,bool PWM_status, int forward_pin, int backward_pin,int Enable_pin);
         ~Motor();

     private: // variables

          ::gpiod::line forward_line_;
          ::gpiod::line backward_line_;
          ::gpiod::line Ena_line_; // for speed control too
          std::string motor_name_;
          bool PWM_status_ = false;

     private: // function 
          

     public: // function
          int init();
          std::string get_motor_name() const;
          bool get_PWM_status() const;
          void forward();
          void backward();
          void set_speed(double pwm_perent);
          void stop_motor();
          void clean_motor();
      
    };
}
#endif