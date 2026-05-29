from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # 一键启动发布节点与订阅节点
    publisher_node = Node(
        package="basic_topic",
        executable="publisher_node",
        name="publisher_node",
        output="screen",
    )

    subscriber_node = Node(
        package="basic_topic",
        executable="subscriber_node",
        name="subscriber_node",
        output="screen",
    )

    return LaunchDescription([
        publisher_node,
        subscriber_node,
    ])
