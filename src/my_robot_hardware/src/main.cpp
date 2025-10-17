#include<iostream>
#include<my_robot_hardware/driver.hpp>

#define RASPBERRY_PY_CHIP_NAME  "gpiochip0"

int main()
{

int ML1_F =12;
int ML1_B =13;
int ML1_EN =18;
std::string Driver_name(RASPBERRY_PY_CHIP_NAME);
bool PMW = false;
std::string motor1{"Left_wheel_1"};
Robot::Driver MyDrivers(Driver_name);

auto M1 = MyDrivers.add_motor(motor1,PMW,ML1_F,ML1_B,ML1_EN);
M1.forward();

}