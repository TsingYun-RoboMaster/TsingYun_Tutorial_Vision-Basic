cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# TODO

# 寻找 C++ 核心通信库
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
# 寻找需要用到的数据结构（四元数）消息库
find_package(geometry_msgs REQUIRED)

# 1. 发布者节点配置
add_executable(publisher_component src/publisher_component.cpp)
# 显式指定头文件搜索路径为项目的 include 目录
target_include_directories(publisher_component PUBLIC include)
ament_target_dependencies(publisher_component rclcpp geometry_msgs rclcpp_components)

# 2. 订阅者节点配置
add_executable(subscriber_component src/subscriber_component.cpp)
# 同样为订阅者显式指定头文件搜索路径
target_include_directories(subscriber_component PUBLIC include)
ament_target_dependencies(subscriber_component rclcpp geometry_msgs rclcpp_components)
# Install
install(TARGETS publisher_component subscriber_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})
# 1. 安装编译好的可执行文件到系统 lib 目录
install(TARGETS
  publisher_component
  subscriber_component
  DESTINATION lib/${PROJECT_NAME}
)

# 2. 安装 launch 文件夹到系统 share 目录
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
