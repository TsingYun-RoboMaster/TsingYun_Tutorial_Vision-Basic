#!/bin/bash
set -e

# Source ROS2 安装环境以及编译好的工作空间
source "/opt/ros/${ROS_DISTRO}/setup.bash"
source "/workspaces/code/install/setup.bash"

# 执行传入的命令（例如 ros2 run ...）
exec "$@"
