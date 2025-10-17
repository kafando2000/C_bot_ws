#include <iostream>
#include <lgpio.h>   
#include <unistd.h>  
#include <thread>    
#include <chrono>    


const int PWM_PIN = 12; 
const int IN1_PIN = 17; 
const int IN2_PIN = 27; 

const int PWM_FREQUENCY = 500; 
const int PWM_RANGE = 255;    

// lgpio chip handle
int g_chip_handle = -1;


bool setup_motor_pins() {
    // Open the gpiochip device
    g_chip_handle = lgGpiochipOpen(0); // 0 typically refers to /dev/gpiochip0
    if (g_chip_handle < 0) {
        std::cerr << "Error: Could not open gpiochip 0. " << lguErrorText(g_chip_handle) << std::endl;
        return false;
    }

    // Claim the direction pins as outputs
    // lgGpioClaimOutput(handle, lFlags, gpio, initial_level)
    if (lgGpioClaimOutput(g_chip_handle, 0, IN1_PIN, 0) < 0) {
        std::cerr << "Error: Could not claim IN1_PIN " << IN1_PIN << ". " << lguErrorText(lgGpioClaimOutput(g_chip_handle,0,IN1_PIN,0)) << std::endl;
        lgGpiochipClose(g_chip_handle);
        return false;
    }
    if (lgGpioClaimOutput(g_chip_handle, 0, IN2_PIN, 0) < 0) {
        std::cerr << "Error: Could not claim IN2_PIN " << IN2_PIN << ". " << lguErrorText(lgGpioClaimOutput(g_chip_handle,0,IN2_PIN,0)) << std::endl;
        lgGpiochipClose(g_chip_handle);
        return false;
    }


    // lgGpioSetPWMFrequency(handle, gpio, freq)
    if (lgGpioSetPWMFrequency(g_chip_handle, PWM_PIN, PWM_FREQUENCY) < 0) {
        std::cerr << "Error: Could not set PWM frequency on pin " << PWM_PIN << ". " << lguErrorText(lgGpioSetPWMFrequency(g_chip_handle,PWM_PIN,PWM_FREQUENCY)) << std::endl;
        lgGpiochipClose(g_chip_handle);
        return false;
    }
    // lgGpioSetPWMRange(handle, gpio, range)
    if (lgGpioSetPWMRange(g_chip_handle, PWM_PIN, PWM_RANGE) < 0) {
        std::cerr << "Error: Could not set PWM range on pin " << PWM_PIN << ". " << lguErrorText(lgGpioSetPWMRange(g_chip_handle,PWM_PIN,PWM_RANGE)) << std::endl;
        lgGpiochipClose(g_chip_handle);
        return false;
    }
    // lgGpioSetPWMDutycycle(handle, gpio, dutycycle)
    if (lgGpioSetPWMDutycycle(g_chip_handle, PWM_PIN, 0) < 0) {
        std::cerr << "Error: Could not set initial PWM duty cycle on pin " << PWM_PIN << ". " << lguErrorText(lgGpioSetPWMDutycycle(g_chip_handle,PWM_PIN,0)) << std::endl;
        lgGpiochipClose(g_chip_handle);
        return false;
    }

    std::cout << "GPIO pins initialized." << std::endl;
    return true;
}

// Function to set motor speed and direction
void set_motor_speed(int speed_percent, int direction) {
    // Ensure speed is within 0-100%
    if (speed_percent < 0) speed_percent = 0;
    if (speed_percent > 100) speed_percent = 100;

    // Convert percentage to duty cycle value
    int duty_cycle = (speed_percent * PWM_RANGE) / 100;

    // Set direction pins
    if (direction == 1) { // Forward
        lgGpioWrite(g_chip_handle, IN1_PIN, 1);
        lgGpioWrite(g_chip_handle, IN2_PIN, 0);
        std::cout << "Forward, ";
    } else if (direction == -1) { // Reverse
        lgGpioWrite(g_chip_handle, IN1_PIN, 0);
        lgGpioWrite(g_chip_handle, IN2_PIN, 1);
        std::cout << "Reverse, ";
    } else { // Stop/Brake (both low is common for braking or coasting)
        lgGpioWrite(g_chip_handle, IN1_PIN, 0);
        lgGpioWrite(g_chip_handle, IN2_PIN, 0);
        std::cout << "Brake, ";
    }

    // Set PWM duty cycle
    lgGpioSetPWMDutycycle(g_chip_handle, PWM_PIN, duty_cycle);
    std::cout << "Speed: " << speed_percent << "% (Duty: " << duty_cycle << ")" << std::endl;
}

// Function to clean up GPIOs
void cleanup_motor_pins() {
    if (g_chip_handle >= 0) {
        // Stop PWM
        lgGpioSetPWMDutycycle(g_chip_handle, PWM_PIN, 0); // Set duty cycle to 0
        lgGpioSetPWMFrequency(g_chip_handle, PWM_PIN, 0); // Stop PWM output

        // Ensure direction pins are low
        lgGpioWrite(g_chip_handle, IN1_PIN, 0);
        lgGpioWrite(g_chip_handle, IN2_PIN, 0);

        // Release the claimed GPIO lines
        lgGpioFree(g_chip_handle, PWM_PIN);
        lgGpioFree(g_chip_handle, IN1_PIN);
        lgGpioFree(g_chip_handle, IN2_PIN);

        // Close the gpiochip
        lgGpiochipClose(g_chip_handle);
        std::cout << "GPIO pins cleaned up and closed." << std::endl;
    }
}

int main() {
    // Setup GPIOs
    if (!setup_motor_pins()) {
        return 1;
    }

    // Set motor to full speed forward for 3 seconds
    std::cout << "Moving motor forward at 100% speed..." << std::endl;
    set_motor_speed(100, 1); 
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Slow down to 50% speed forward for 3 seconds
    std::cout << "Slowing down to 50% speed forward..." << std::endl;
    set_motor_speed(50, 1); // 50% speed, forward
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Stop motor for 2 seconds
    std::cout << "Stopping motor..." << std::endl;
    set_motor_speed(0, 0); // 0% speed, stop/brake
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Move motor reverse at 75% speed for 3 seconds
    std::cout << "Moving motor reverse at 75% speed..." << std::endl;
    set_motor_speed(75, -1); // 75% speed, reverse
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Stop motor and cleanup
    std::cout << "Stopping motor and cleaning up..." << std::endl;
    set_motor_speed(0, 0); // Ensure motor is off
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Give it a moment

    cleanup_motor_pins();

    return 0;
}