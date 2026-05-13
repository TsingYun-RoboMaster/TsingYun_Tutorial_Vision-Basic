from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():
    container = ComposableNodeContainer(
        name='subscriber_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::SubscriberComponent',
                name='subscriber_node'),
        ],
        output='screen',
    )

    return LaunchDescription([container])