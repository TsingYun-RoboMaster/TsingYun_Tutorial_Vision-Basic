#ifndef BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
#define BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <chrono>
#include <cmath>
#include <memory>
#include <string>
#include <functional>
namespace basic_topic
{

    class PublisherComponent : public rclcpp::Node
    {
    public:
        explicit PublisherComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        static constexpr double kPi = 3.14159265358979323846;
        double normalize_angle(double angle);                                                     // 角度归一
        geometry_msgs::msg::Quaternion rpy_to_quaternion(double roll, double pitch, double yaw);  // 角度转四元数

        // TODO
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;  // 发布
        rclcpp::TimerBase::SharedPtr timer_;                                      // 定时器
        double time_sec_{0.0};

        void publish_quaternion();  // 回调函数
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
