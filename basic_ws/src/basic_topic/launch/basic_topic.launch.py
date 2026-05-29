from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='basic_topic',
            executable='publisher_component',
            name='publisher_node',
            output='screen',
        ),
        Node(
            package='basic_topic',
            executable='subscriber_component',
            name='subscriber_node',
            output='screen',
        ),
    ])