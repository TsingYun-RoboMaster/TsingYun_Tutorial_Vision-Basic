from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # TODO
    # pass
    return LaunchDescription([
            # 启动发布者节点
            Node(
                package='basic_topic',
                executable='publisher_component',
                name='publisher_node',
                output='screen'
            ),
            # 启动订阅者节点
            Node(
                package='basic_topic',
                executable='subscriber_component',
                name='subscriber_node',
                output='screen'
            )
        ])