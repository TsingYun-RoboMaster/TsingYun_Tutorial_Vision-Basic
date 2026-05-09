#ifndef BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
#define BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <chrono>
#include <cstddef>  // std::size_t 用来记录当前发布的是第几组样例数据。
#include <cmath>
#include <memory>
#include <string>

namespace basic_topic
{

    class PublisherComponent : public rclcpp::Node
    {
    public:
        explicit PublisherComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        // 圆周率常量用于角度归一化，把角度统一压到 [-pi, pi]。
        static constexpr double kPi = 3.14159265358979323846;
        // 发布端和订阅端必须使用完全相同的话题名，否则两个节点无法通信。
        static constexpr const char* kTopicName = "/quaternion_topic";

        // 将任意弧度角归一化到 [-pi, pi]，便于终端输出时直接对比。
        double normalize_angle(double angle);
        // 把 Roll/Pitch/Yaw 欧拉角转换为 ROS 消息要求的四元数格式。
        geometry_msgs::msg::Quaternion rpy_to_quaternion(double roll, double pitch, double yaw);
        // 定时器每触发一次，就调用这个函数生成并发布一条四元数消息。
        void publish_quaternion();

        // ROS2 publisher 句柄，负责向 /quaternion_topic 发送 Quaternion 消息。
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;
        // 定时器句柄必须保存为成员变量，否则构造函数结束后定时器会被释放。
        rclcpp::TimerBase::SharedPtr timer_;
        // 样例序号用于生成一组随时间变化但可复现的 RPY 数据。
        std::size_t sample_index_;
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
