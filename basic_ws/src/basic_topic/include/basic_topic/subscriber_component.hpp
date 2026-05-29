#ifndef BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
#define BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <cmath>
#include <memory>
#include <string>

namespace basic_topic
{

    class SubscriberComponent : public rclcpp::Node
    {
    public:
        explicit SubscriberComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        static constexpr double kPi = 3.14159265358979323846;
        double normalize_angle(double angle);
        void quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q, double& roll, double& pitch, double& yaw);

        // 订阅回调：接收四元数，转换为 RPY 并打印
        void topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg);

        // 订阅者，话题类型为 geometry_msgs/msg/Quaternion
        rclcpp::Subscription<geometry_msgs::msg::Quaternion>::SharedPtr subscription_;
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
