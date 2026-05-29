from launch import LaunchDescription

from launch_ros.actions import ComposableNodeContainer

from launch_ros.descriptions import ComposableNode


def generate_launch_description():

    publisher_node = ComposableNode(
        package='basic_topic',
        plugin='basic_topic::PublisherComponent',
        name='publisher_node'
    )

    subscriber_node = ComposableNode(
        package='basic_topic',
        plugin='basic_topic::SubscriberComponent',
        name='subscriber_node'
    )

    container = ComposableNodeContainer(
        name='basic_topic_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            publisher_node,
            subscriber_node
        ],
        output='screen'
    )

    return LaunchDescription([
        container
    ])