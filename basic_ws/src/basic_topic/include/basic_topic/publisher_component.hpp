#ifndef BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
#define BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_

// ============================================================
// ROS2 核心头文件
// rclcpp/rclcpp.hpp: ROS2 C++ 客户端库，提供 Node、Publisher、Timer 等
// rclcpp_components/register_node_macro.hpp: 组件注册宏，使节点可作为组件动态加载
// geometry_msgs/msg/quaternion.hpp: 四元数消息类型（x, y, z, w）
// ============================================================
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <chrono>   // 时间相关（std::chrono_literals）
#include <cmath>    // 数学函数（cos, sin, fmod 等）
#include <memory>   // 智能指针（std::make_shared）
#include <string>   // 字符串支持

namespace basic_topic
{

    // ============================================================
    // PublisherComponent 类 — 发布者节点
    //
    // 继承自 rclcpp::Node（ROS2 节点基类）
    // 功能：周期性地生成四元数消息并发布到 "quaternion" 话题
    // ============================================================
    class PublisherComponent : public rclcpp::Node
    {
    public:
        // 构造函数：NodeOptions 允许以组件形式动态加载时传入配置参数
        // 默认参数 rclcpp::NodeOptions() 表示使用默认配置
        explicit PublisherComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        // π 的精确值，static constexpr 表示编译期常量，不占运行时内存
        static constexpr double kPi = 3.14159265358979323846;

        // ---- 工具函数 ----

        // 将任意角度归一化到 [-π, π] 范围内
        // 例如：450°（= 2.5π rad）→ 归一化为 0.5π rad（90°）
        double normalize_angle(double angle);

        // 将 Roll（横滚）、Pitch（俯仰）、Yaw（偏航）转换为四元数
        // 这是旋转变换的常用需求：RPY → Quaternion
        geometry_msgs::msg::Quaternion rpy_to_quaternion(double roll, double pitch, double yaw);

        // ---- ROS2 核心对象 ----

        // 发布者：向话题 "quaternion" 发送 geometry_msgs::msg::Quaternion 消息
        // SharedPtr 是 ROS2 中常用的智能指针，自动管理内存
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;

        // 定时器：周期性触发回调函数（本例中每 500ms 触发一次）
        rclcpp::TimerBase::SharedPtr timer_;

        // 定时器回调函数：生成、打印并发布四元数消息
        void timer_callback();
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
