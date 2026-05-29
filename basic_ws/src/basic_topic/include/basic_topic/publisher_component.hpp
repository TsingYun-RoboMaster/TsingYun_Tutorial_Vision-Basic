#ifndef BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
#define BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <chrono>
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
        static constexpr double kPi = 3.14159265358979323846;
        double normalize_angle(double angle);
        geometry_msgs::msg::Quaternion rpy_to_quaternion(double roll, double pitch, double yaw);

        // 定时回调：生成一组 RPY，转换为四元数并发布，同时在终端打印 RPY
        void timer_callback();

        // 发布者，话题类型为 geometry_msgs/msg/Quaternion
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;
        // 定时器，周期性触发发布
        rclcpp::TimerBase::SharedPtr timer_;
        // 当前的 RPY 值，每次发布后自增，让发布内容动态变化
        double roll_{0.0};
        double pitch_{0.0};
        double yaw_{0.0};
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
