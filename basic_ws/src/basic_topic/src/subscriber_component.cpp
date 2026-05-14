#include "basic_topic/subscriber_component.hpp"
#include <functional>
#include <cmath>

namespace basic_topic
{

SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options)
: Node("subscriber_node", options)
{
    subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
        "quat_topic",
        10,
        std::bind(&SubscriberComponent::subscription_callback, this, std::placeholders::_1)
    );
}



double SubscriberComponent::normalize_angle(double angle)
{
    angle = std::fmod(angle + kPi, 2.0 * kPi);
    if (angle < 0.0)
        angle += 2.0 * kPi;
    return angle - kPi;
}

void SubscriberComponent::quaternion_to_rpy(
    const geometry_msgs::msg::Quaternion& q,
    double& roll, double& pitch, double& yaw)
{
    const double sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
    const double cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
    roll = std::atan2(sinr_cosp, cosr_cosp);

    const double sinp = 2.0 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1.0)
        pitch = std::copysign(kPi / 2.0, sinp);
    else
        pitch = std::asin(sinp);

    const double siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
    const double cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
    yaw = std::atan2(siny_cosp, cosy_cosp);
}

void SubscriberComponent::subscription_callback(
    const geometry_msgs::msg::Quaternion::SharedPtr msg)
{
    double roll, pitch, yaw;
    quaternion_to_rpy(*msg, roll, pitch, yaw);

    // To make RPY differ by π, add π to each
    roll += kPi;
    pitch += kPi;
    yaw += kPi;

    RCLCPP_INFO(this->get_logger(),
        "Received Quaternion: [%.3f, %.3f, %.3f, %.3f] | RPY: [%.3f, %.3f, %.3f]",
        msg->x, msg->y, msg->z, msg->w,
        roll, pitch, yaw
    );
}

} // namespace basic_topic

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<basic_topic::SubscriberComponent>());
    rclcpp::shutdown();
    return 0;
}
