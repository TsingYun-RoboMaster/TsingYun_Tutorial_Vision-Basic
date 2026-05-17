#ifndef BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
#define BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_

// ============================================================
// ROS2 核心头文件
// rclcpp/rclcpp.hpp: ROS2 C++ 客户端库，提供 Node、Subscription 等
// rclcpp_components/register_node_macro.hpp: 组件注册宏
// geometry_msgs/msg/quaternion.hpp: 四元数消息类型
// ============================================================
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <cmath>    // 数学函数（atan2, asin, fmod 等）
#include <memory>   // 智能指针
#include <string>   // 字符串支持

namespace basic_topic
{

    // ============================================================
    // SubscriberComponent 类 — 订阅者节点
    //
    // 继承自 rclcpp::Node（ROS2 节点基类）
    // 功能：订阅 "quaternion" 话题，接收四元数消息并转换为 RPY 打印
    // ============================================================
    class SubscriberComponent : public rclcpp::Node
    {
    public:
        // 构造函数：NodeOptions 允许以组件形式动态加载
        explicit SubscriberComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        // π 的精确值
        static constexpr double kPi = 3.14159265358979323846;

        // ---- 工具函数 ----

        // 将任意角度归一化到 [-π, π] 范围
        double normalize_angle(double angle);

        // 将四元数转换为 Roll（横滚）、Pitch（俯仰）、Yaw（偏航）
        // 这是 rpy_to_quaternion 的逆运算：Quaternion → RPY
        // 注意：使用引用参数（double&）来返回多个值
        void quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q,
                               double& roll, double& pitch, double& yaw);

        // ---- ROS2 核心对象 ----

        // 订阅者：监听 "quaternion" 话题，收到消息时自动调用回调函数
        rclcpp::Subscription<geometry_msgs::msg::Quaternion>::SharedPtr subscription_;

        // 话题回调函数：每当有新消息到达时被调用
        // SharedPtr 是消息的共享指针，避免复制大对象
        void topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg);
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
