from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # TODO: 启动发布节点和订阅节点
    return LaunchDescription([
        Node(
            package='basic_topic',
            executable='publisher_node',
            name='publisher_node',
            output='screen',
        ),
        Node(
            package='basic_topic',
            executable='subscriber_node',
            name='subscriber_node',
            output='screen',
        ),
    ])