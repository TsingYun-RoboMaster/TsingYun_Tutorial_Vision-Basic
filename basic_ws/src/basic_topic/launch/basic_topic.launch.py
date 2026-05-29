from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # TODO
    return LaunchDescription([
        # 启动发布者节点
        Node(
            package='basic_topic',
            executable='publisher_node',
            name='publisher_component',
            output='screen'
        ),
        
        # 启动订阅者节点
        Node(
            package='basic_topic',
            executable='subscriber_node',
            name='subscriber_component',
            output='screen'
        )
    ])