# 作业说明

## 作业描述

本次任务需要基于 ROS2 框架和 Docker 容器完成一个简单的两节点之间互相通信的 C++ 代码实现。

## 作业要求

1. 使用 C++ 编写 ROS2 节点，包含一个发布节点 `publisher_component` 和一个订阅节点 `subscriber_component`
   1. 话题类型使用 `geometry_msgs/msg/Quaternion`
   2. 发布节点生成四元数消息，并在终端打印对应的 Roll、Pitch、Yaw 值
   3. 订阅节点接收四元数后转换为 Roll、Pitch、Yaw，并打印四元数与 Roll、Pitch、Yaw 值
   4. 两边的 Roll、Pitch、Yaw 值需要相差 $\pi$ 的整数倍
2. 编写 `basic_topic.launch.py` 文件，支持一键启动两个节点
3. 编写 Docker 配置文件，支持两种运行模式
	1. Dev 模式：编写 `Dockerfile.dev`，作为主要开发环境，需要在容器内尝试通过编译以及一键启动两个节点，录制效果视频
	2. Deploy 模式：编写 `Dockerfile.deploy` 和 `docker-compose.deploy.yml`，要求发布与订阅节点分别运行在两个容器中，并且能够实现只执行 `docker compose -f docker/docker-compose.deploy.yml up -d` 后两个容器能够互相通信，录制效果视频

## 需要修改的目录与文件

- 主要功能包：[`basic_topic`](../basic_ws/src/basic_topic)
  - 发布节点：[`publisher_component.cpp`](../basic_ws/src/basic_topic/publisher_component.cpp)、[`publisher_component.hpp`](../basic_ws/src/basic_topic/include/basic_topic/publisher_component.hpp)
  - 订阅节点：[`subscriber_component.cpp`](../basic_ws/src/basic_topic/subscriber_component.cpp)、[`subscriber_component.hpp`](../basic_ws/src/basic_topic/include/basic_topic/subscriber_component.hpp)
  - 启动文件：[`basic_topic.launch.py`](../basic_ws/src/basic_topic/launch/basic_topic.launch.py)
  - 编译配置：[`CMakeLists.txt`](../basic_ws/src/basic_topic/CMakeLists.txt)、[`package.xml`](../basic_ws/src/basic_topic/package.xml)
- Docker 相关文件：[`docker`](../docker)
  - Dev 容器：[`Dockerfile.dev`](../docker/Dockerfile.dev)
  - Deploy 容器：[`Dockerfile.deploy`](../docker/Dockerfile.deploy)、[`docker-compose.deploy.yml`](../docker/docker-compose.deploy.yml)

- 可看需求增加其他文件

## 可能用到的指令

编译 ROS2 包：

```bash
source /opt/ros/${ROS_DISTRO}/setup.bash
colcon build --symlink-install
source install/setup.bash
```

启动 ROS2 节点：

```bash
ros2 run <package_name> <executable_name>
ros2 launch <package_name> <launch_file.launch.py>
```

ROS2 调试：

```bash
ros2 topic list # 列出当前所有话题
ros2 topic echo <topic_name> # 监听话题消息
ros2 node list # 列出当前所有节点
ros2 node info <node_name> # 查看节点信息
```

Docker 部署：

```bash
docker compose -f <docker-compose-file.yml> up -d
```

Docker 调试：

```bash
docker exec -it <container_name> bash # 进入容器
docker logs -f <container_name> # 查看容器日志
```

## Checkpoints

如果你在完成作业过程中遇到困难，可以逐步完成以下 Checkpoints：

- [ ] 在 Dev 容器中编译 ROS2 包，并且能够看到编译成功的信息
- [ ] 在 Dev 容器中分别启动 ROS2 节点，并且能够看到正确的通信信息
- [ ] 在 Dev 容器中一键启动 ROS2 节点，并且能够看到正确的通信信息
- [ ] 使用 Deploy 容器一键部署 ROS2 节点，并且能够看到正确的通信信息