#ifndef BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
#define BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <cmath>
#include <memory>
#include <string>
#include <functional>
namespace basic_topic
{

    class SubscriberComponent : public rclcpp::Node
    {
    public:
        explicit SubscriberComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        static constexpr double kPi = 3.14159265358979323846;
        double normalize_angle(double angle);                                                                       // 角度归一
        void quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q, double& roll, double& pitch, double& yaw);  // 四元数转欧拉角

        // TODO
        rclcpp::Subscription<geometry_msgs::msg::Quaternion>::SharedPtr subscription_;  // 订阅
        void quaternion_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg);  // 回调函数
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
