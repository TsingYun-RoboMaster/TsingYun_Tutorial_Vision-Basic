# ============================================================
# basic_topic.launch.py — ROS2 Launch 启动文件
#
# 功能：一键启动发布者节点和订阅者节点
# 使用方式：ros2 launch basic_topic basic_topic.launch.py
#
# 启动方式说明：
# 这里使用了"可组合节点容器"（ComposableNodeContainer）方式，
# 将发布者和订阅者作为"组件"加载到同一个进程的同一个容器中。
#
# 优点：
# 1. 两个节点在同一个进程内通信，零拷贝、零延迟
# 2. 比启动两个独立进程更高效（省去进程间通信开销）
# 3. 适用于开发模式（Dev）的快速验证
#
# 部署模式（Deploy）使用独立可执行文件 ros2 run 分别启动，
# 对应 docker-compose.deploy.yml 中的配置。
# ============================================================

from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():
    # ---- 创建可组合节点容器 ----
    #
    # ComposableNodeContainer 是一个特殊的 ROS2 节点，
    # 它本身是一个"组件容器"，可以动态加载多个组件。
    #
    # 参数说明：
    #   name:     容器节点的名称（ros2 node list 中显示的名字）
    #   namespace: 命名空间（空字符串表示全局命名空间）
    #   package:  容器可执行文件所在的包（rclcpp_components 是 ROS2 标准包）
    #   executable: 容器可执行文件（component_container_mt = 多线程版本）
    #   composable_node_descriptions: 要加载的组件列表
    #   output:   日志输出方式（'screen' = 打印到终端）
    container = ComposableNodeContainer(
        name='basic_topic_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container_mt',
        composable_node_descriptions=[
            # ---- 发布者组件 ----
            # package:  组件所在的 ROS2 包名
            # plugin:   组件的 C++ 类名（必须与注册宏中的名字一致）
            # name:     加载后节点的名称
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::PublisherComponent',
                name='publisher_node',
            ),
            # ---- 订阅者组件 ----
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::SubscriberComponent',
                name='subscriber_node',
            ),
        ],
        output='screen',
    )

    # LaunchDescription 是 ROS2 Launch 系统的核心概念，
    # 它包含一组"动作"（Action），这里只有一个容器启动动作。
    # 返回 LaunchDescription 后，ros2 launch 会按照描述执行启动。
    return LaunchDescription([container])