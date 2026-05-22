from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # 1. 声明发布者节点
    # package: 你的功能包名字，对应 project(basic_topic)
    # executable: 对应 CMakeLists.txt 里 rclcpp_components_register_node 中指定的 EXECUTABLE 名字
    # output: 'screen' 意思是让节点的日志信息直接打印到当前终端屏幕上
    publisher_node = Node(
        package='basic_topic',
        executable='publisher_node',
        name='publisher_node_instance',
        output='screen'
    )

    # 2. 声明订阅者节点
    subscriber_node = Node(
        package='basic_topic',
        executable='subscriber_node',
        name='subscriber_node_instance',
        output='screen'
    )

    # 3. 将这两个节点打包进 LaunchDescription 统一管理
    return LaunchDescription([
        publisher_node,
        subscriber_node
    ])