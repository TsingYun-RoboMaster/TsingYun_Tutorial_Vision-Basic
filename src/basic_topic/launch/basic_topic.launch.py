import launch
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

def generate_launch_description():
    """Generate launch description with components."""
    
    # 定义容器，将发布者和订阅者作为组件放入其中
    container = ComposableNodeContainer(
        name='my_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::PublisherComponent',
                name='publisher_node'),
            ComposableNode(
                package='basic_topic',
                plugin='basic_topic::SubscriberComponent',
                name='subscriber_node')
        ],
        output='screen',
    )

    # 【关键！】必须返回这个 LaunchDescription 对象
    return launch.LaunchDescription([container])