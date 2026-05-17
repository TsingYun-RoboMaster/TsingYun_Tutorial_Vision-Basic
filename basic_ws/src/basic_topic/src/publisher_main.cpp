// ============================================================
// publisher_main.cpp — 发布者独立可执行文件入口
//
// 用途：在不使用组件容器的情况下，直接运行发布者节点
// 用于 Docker 部署模式（docker-compose.deploy.yml）
//
// 与组件方式的区别：
//   - 组件方式：编译为 .so 共享库，由 component_container 动态加载
//   - 独立方式：编译为可执行文件，直接由 ros2 run 启动
//   两种方式使用相同的 PublisherComponent 类，代码完全复用
// ============================================================

#include "basic_topic/publisher_component.hpp"

int main(int argc, char* argv[])
{
    // ---- 1. 初始化 ROS2 ----
    // rclcpp::init 必须在所有 ROS2 调用之前执行
    // 它解析命令行参数（如 --ros-args），初始化通信层
    rclcpp::init(argc, argv);

    // ---- 2. 创建并旋转节点 ----
    // rclcpp::spin 是 ROS2 的事件循环（类似 while(1) 但更高效）
    // 它在内部：
    //   a) 等待定时器触发
    //   b) 处理发布队列
    //   c) 处理订阅回调
    //   d) 处理服务请求
    // spin 会一直阻塞，直到收到 Ctrl+C 信号或 rclcpp::shutdown()
    //
    // std::make_shared<PublisherComponent>() 创建节点对象，
    // 使用默认的 NodeOptions（因为不涉及组件加载）
    rclcpp::spin(std::make_shared<basic_topic::PublisherComponent>());

    // ---- 3. 清理并退出 ----
    // 当 spin 返回时（用户按 Ctrl+C），关闭 ROS2 通信层
    rclcpp::shutdown();
    return 0;
}
