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
        // 1. 声明一个发布者，发布的数据类型是 geometry_msgs::msg::Quaternion
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;

        // 2. 声明一个定时器，用来规律地触发发布动作
        rclcpp::TimerBase::SharedPtr timer_;

        // 3. 声明三个变量，用来记录和递增当前的 Roll, Pitch, Yaw 角度
        double roll_ = 0.0;
        double pitch_ = 0.0;
        double yaw_ = 0.0;

        // 4. 定时器绑定的回调函数，每次时间到了就会执行它
        void timer_callback();
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_