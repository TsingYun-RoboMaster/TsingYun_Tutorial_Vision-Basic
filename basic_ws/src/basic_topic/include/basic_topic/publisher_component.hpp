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

        // TODO
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;
        
        // 声明定时器指针
        rclcpp::TimerBase::SharedPtr timer_;
        
        // 声明定时器触发时要调用的回调函数
        void timer_callback();
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
