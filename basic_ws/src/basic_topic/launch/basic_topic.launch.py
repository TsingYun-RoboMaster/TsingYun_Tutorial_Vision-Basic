from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

def generate_launch_description():
    container = ComposableNodeContainer(
        name='component_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::PublisherComponent',
                name='topic_publisher'
            ),
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::SubscriberComponent',
                name='topic_subscriber'
            ),
        ],
        output='screen',
    )
    return LaunchDescription([container])