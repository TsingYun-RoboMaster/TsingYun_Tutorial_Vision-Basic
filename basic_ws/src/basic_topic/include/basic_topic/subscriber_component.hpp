#ifndef BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
#define BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <cmath>
#include <memory>

namespace basic_topic
{

class SubscriberComponent : public rclcpp::Node
{
public:
    explicit SubscriberComponent(
        const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

private:
    static constexpr double kPi = 3.14159265358979323846;

    // callback
    void subscription_callback(
        const geometry_msgs::msg::Quaternion::SharedPtr msg);

    // utils
    double normalize_angle(double angle);

    void quaternion_to_rpy(
        const geometry_msgs::msg::Quaternion& q,
        double& roll, double& pitch, double& yaw);

    rclcpp::Subscription<geometry_msgs::msg::Quaternion>::SharedPtr subscription_;
};

}  // namespace basic_topic

#endif  // BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_