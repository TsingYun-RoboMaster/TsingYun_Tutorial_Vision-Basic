from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # 1. 声明发布者节点进程
    publisher_node = Node(
        package='basic_topic',             # 功能包的名称（与 package.xml 中一致）
        executable='publisher_component',  # 可执行文件的名称（注意：必须与 CMakeLists.txt 中 add_executable 定义的目标名称完全一致）
        name='publisher_node',             # 节点在 ROS 2 拓扑图中的实际运行名称
        output='screen'                    # 将节点内部通过 RCLCPP_INFO 打印的日志标准输出（stdout）重定向到当前终端屏幕
    )

    # 2. 声明订阅者节点进程
    subscriber_node = Node(
        package='basic_topic',
        executable='subscriber_component', # 同上，需与编译生成的二进制文件名一致
        name='subscriber_node',
        output='screen'
    )

    # 3. 将所有声明的节点作为列表传入 LaunchDescription，交由底层调度器并发执行
    return LaunchDescription([
        publisher_node,
        subscriber_node
    ])