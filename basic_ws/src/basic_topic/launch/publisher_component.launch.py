from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():
    container = ComposableNodeContainer(
        name='publisher_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::PublisherComponent',
                name='publisher_node'),
        ],
        output='screen',
    )

    return LaunchDescription([container])