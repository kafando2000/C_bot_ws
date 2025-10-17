#include <gpiod.hpp>
#ifndef ROBOT_DEVICE__HPP
#define ROBOT_DEVICE__HPP
 namespace Robot{
// Device like Raspberry Pi or Arduino
    class Device{
        public:
            Device(std::string device_name);
            ~Device();
            inline std::string get_device_name() const{
                return this->device_name_;

            }

            inline ::gpiod::chip get_device_chip() const {
                return this->chip_;
            }
        private:
            ::gpiod::chip chip_;
            std::string device_name_;
 };
}

#endif
