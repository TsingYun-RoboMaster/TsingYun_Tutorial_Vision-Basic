// ============================================================
// subscriber_main.cpp — 订阅者独立可执行文件入口
//
// 用途：在不使用组件容器的情况下，直接运行订阅者节点
// 用于 Docker 部署模式（docker-compose.deploy.yml）
//
// 结构与 publisher_main.cpp 完全相同，只是节点类不同
// ============================================================

#include "basic_topic/subscriber_component.hpp"

int main(int argc, char* argv[])
{
    // 初始化 ROS2 通信层
    rclcpp::init(argc, argv);

    // 创建订阅者节点并进入事件循环
    // spin 会一直运行，处理消息回调，直到被中断
    rclcpp::spin(std::make_shared<basic_topic::SubscriberComponent>());

    // 清理并退出
    rclcpp::shutdown();
    return 0;
}
