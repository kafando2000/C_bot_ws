#include "my_robot_hardware/driver.hpp"
#include <gpiod.hpp>
#include<iostream>
#include<thread>
#include<chrono>
namespace Robot{

    Motor::Motor(::Robot::Driver& motordriver,std::string motor_name,bool PWM_status, int forward_pin, int backward_pin,int Enable_pin){
        this->motor_name_ = motor_name;
        this->PWM_status_ =PWM_status;

        this->forward_line_ = motordriver.get_device_chip().get_line(forward_pin);
        this->backward_line_ = motordriver.get_device_chip().get_line(backward_pin); 
        this->Ena_line_ = motordriver.get_device_chip().get_line( Enable_pin); 
    }

     std::string Motor::get_motor_name() const{
        return this->motor_name_;
     }

     bool Motor::get_PWM_status() const {
        return this->PWM_status_;
     }

    int Motor::init(){
        try{
            this->forward_line_.request({(this->get_motor_name()+"forward").c_str(),::gpiod::line::DIRECTION_OUTPUT,0});
            this->backward_line_.request({(this->get_motor_name()+"backward").c_str(),::gpiod::line::DIRECTION_OUTPUT,0});

            this->Ena_line_.request({(this->get_motor_name()+"Enable").c_str(),::gpiod::line::DIRECTION_OUTPUT,0});
            return 1;
        }
        catch(const std::exception& e){
            std::cerr<<"error requesting lines as output"<<e.what()<<"\n";
            if (forward_line_.is_requested()) forward_line_.release();
            if (backward_line_.is_requested()) backward_line_.release();
            if (Ena_line_.is_requested()) Ena_line_.release();
            return 0;
        }
    }
    Motor::Motor(){
        
    }


    // start motor and direction at the same time if PMW control is false, otherwise set just the direction
    void Motor::
    forward(){
        if(this->init() && this->get_PWM_status()){
            this->forward_line_.set_value(1);
            this->backward_line_.set_value(0);
            this->Ena_line_.set_value(1);
            
        }
        else{
            this->forward_line_.set_value(1);
            this->backward_line_.set_value(0);
        }

    }

    // start motor and direction at the same time if PMW control is false, otherwise set just the direction
    void Motor::
    backward(){
        if(this->init() && this->get_PWM_status()){
            this->forward_line_.set_value(0);
            this->backward_line_.set_value(1);
            this->Ena_line_.set_value(1);
            
        }
        else{
            this->forward_line_.set_value(0);
            this->backward_line_.set_value(1);
        }

    }

    void Motor::
    set_speed(double percent_effort){
        if(this->PWM_status_){
    
            long duty_whole_time = 1000000; // micro second
            if (percent_effort <= 0) Ena_line_.set_value(0);
            if (percent_effort >= 0) Ena_line_.set_value(1);
            else{
                long duty_on_time = duty_whole_time*percent_effort/100;
                long duty_off_time = duty_whole_time - duty_on_time;
                while(true){
                    Ena_line_.set_value(1);
                    std::this_thread::sleep_for(std::chrono::microseconds(duty_on_time));
                    Ena_line_.set_value(0);
                    std::this_thread::sleep_for(std::chrono::microseconds(duty_off_time));
                }
            }
        }
    }
    // stop the motor 
    void Motor::stop_motor(){
        this->backward_line_.set_value(0);
        this->forward_line_.set_value(0);
        this->Ena_line_.set_value(0);

    }
    // release the motor lines
    void Motor::clean_motor(){
        if (forward_line_.is_requested()) forward_line_.release();
        if (backward_line_.is_requested()) backward_line_.release();
        if (Ena_line_.is_requested()) Ena_line_.release();
    }
    
    Motor::~Motor(){
        clean_motor();

    }
}