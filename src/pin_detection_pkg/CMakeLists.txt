cmake_minimum_required(VERSION 3.8)
project(pin_detection_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# we need PkgConfig for the command below
find_package(PkgConfig REQUIRED)

# stores the location of libgpiod into GPIOD
pkg_check_modules(GPIODCXX REQUIRED libgpiodcxx)

add_executable(pin_publisher src/pin_publisher.cpp)
add_executable(pin_subscriber src/pin_subscriber.cpp)

target_include_directories(pin_publisher PUBLIC ${GPIODCXX_INCLUDE_DIRS})
target_link_libraries(pin_publisher ${GPIODCXX_LIBRARIES})

ament_target_dependencies(pin_publisher rclcpp std_msgs)
ament_target_dependencies(pin_subscriber rclcpp std_msgs)

install(TARGETS
  pin_publisher
  pin_subscriber
  DESTINATION lib/${PROJECT_NAME})

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
