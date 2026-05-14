#include "basic_topic/subscriber_component.hpp"
#include <functional>
#include <iostream>

namespace basic_topic
{

SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options)
    : Node("subscriber_node", options)
{
    subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
        "quaternion_topic", 10,
        std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "Subscriber node has been started.");
}

void SubscriberComponent::topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg)
{
    double roll_orig = 0.0;
    double pitch_orig = 0.0;
    double yaw_orig = 0.0;
    quaternion_to_rpy(*msg, roll_orig, pitch_orig, yaw_orig);

    const double roll_pi = normalize_angle(roll_orig + kPi);
    const double pitch_pi = normalize_angle(pitch_orig + kPi);
    const double yaw_pi = normalize_angle(yaw_orig + kPi);

    std::cout << "[Subscriber] Received Quaternion: w=" << msg->w
              << ", x=" << msg->x
              << ", y=" << msg->y
              << ", z=" << msg->z << std::endl;
    std::cout << "[Subscriber] Roll=" << roll_orig
              << ", Pitch=" << pitch_orig
              << ", Yaw=" << yaw_orig << std::endl;
    std::cout << "[Subscriber] Roll (after +pi)=" << roll_pi
              << ", Pitch (after +pi)=" << pitch_pi
              << ", Yaw (after +pi)=" << yaw_pi << std::endl;
}

double SubscriberComponent::normalize_angle(double angle)
{
    angle = std::fmod(angle + kPi, 2.0 * kPi);
    if (angle < 0.0)
        angle += 2.0 * kPi;
    return angle - kPi;
}

void SubscriberComponent::quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q,
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

}  // namespace basic_topic

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<basic_topic::SubscriberComponent>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
