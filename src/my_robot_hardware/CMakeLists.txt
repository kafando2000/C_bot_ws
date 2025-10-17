cmake_minimum_required(VERSION 3.8)
project(my_robot_hardware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
install(
   DIRECTORY include/${PROJECT_NAME}/
   DESTINATION share/${PROJECT_NAME}/

)
add_library(device STATIC src/device.cpp include/my_robot_hardware/device.hpp)
add_library(motor STATIC src/motor.cpp include/my_robot_hardware/motor.hpp)
add_library(driver STATIC  src/driver.cpp include/my_robot_hardware/driver.hpp)

target_include_directories( device PUBLIC include)
target_include_directories( motor PUBLIC include)
target_include_directories( driver PUBLIC include)

target_compile_features(device PUBLIC cxx_std_17)
target_compile_features(motor PUBLIC cxx_std_17)
target_compile_features(driver PUBLIC cxx_std_17)

add_executable(DCDriver src/main.cpp)
target_link_libraries(DCDriver PUBLIC device motor driver -l:libgpiodcxx.a -l:libgpiod.a ) 
install(TARGETS DCDriver
  DESTINATION lib/${PROJECT_NAME}
)

# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
