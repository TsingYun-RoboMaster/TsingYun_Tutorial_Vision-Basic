from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # publisher_node 对应 ros2 run basic_topic publisher_component。
    publisher_node = Node(
        # package 指明可执行文件属于 basic_topic 包。
        package="basic_topic",
        # executable 必须和 CMakeLists.txt 中 add_executable 的目标名一致。
        executable="publisher_component",
        # name 是 ROS2 节点运行时显示出来的名字。
        name="publisher_node",
        # output='screen' 会把日志直接打印到当前终端，方便检查和录屏。
        output="screen",
    )

    # subscriber_node 对应 ros2 run basic_topic subscriber_component。
    subscriber_node = Node(
        # 两个节点在同一个 package 中，只是启动不同 executable。
        package="basic_topic",
        # executable 必须和 CMakeLists.txt 中 add_executable 的目标名一致。
        executable="subscriber_component",
        # name 是 ROS2 节点运行时显示出来的名字。
        name="subscriber_node",
        # 订阅端也输出到屏幕，便于直接看到接收和转换结果。
        output="screen",
    )

    # LaunchDescription 把两个 Node 组合成一个 launch 文件，实现一键启动。
    return LaunchDescription([publisher_node, subscriber_node])
