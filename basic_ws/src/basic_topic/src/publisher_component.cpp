#include "basic_topic/publisher_component.hpp"
#include <functional>
#include <iostream>

namespace basic_topic
{

PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options)
    : Node("publisher_node", options), yaw_angle_(0.0)
{
    publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion_topic", 10);
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(500),
        std::bind(&PublisherComponent::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "Publisher node has been started.");
}

double PublisherComponent::normalize_angle(double angle)
{
    angle = std::fmod(angle + kPi, 2.0 * kPi);
    if (angle < 0.0)
        angle += 2.0 * kPi;
    return angle - kPi;
}

geometry_msgs::msg::Quaternion PublisherComponent::rpy_to_quaternion(
    double roll, double pitch, double yaw)
{
    geometry_msgs::msg::Quaternion q;
    const double half_roll = roll * 0.5;
    const double half_pitch = pitch * 0.5;
    const double half_yaw = yaw * 0.5;

    const double cr = std::cos(half_roll);
    const double sr = std::sin(half_roll);
    const double cp = std::cos(half_pitch);
    const double sp = std::sin(half_pitch);
    const double cy = std::cos(half_yaw);
    const double sy = std::sin(half_yaw);

    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;
    return q;
}

void PublisherComponent::timer_callback()
{
    const double roll = 0.0;
    const double pitch = 0.0;
    const double yaw = yaw_angle_;

    const auto message = rpy_to_quaternion(roll, pitch, yaw);
    publisher_->publish(message);

    std::cout << "[Publisher] Published Quaternion: w=" << message.w
              << ", x=" << message.x
              << ", y=" << message.y
              << ", z=" << message.z << std::endl;
    std::cout << "[Publisher] Roll=" << roll
              << ", Pitch=" << pitch
              << ", Yaw=" << yaw << std::endl;

    yaw_angle_ = normalize_angle(yaw_angle_ + 0.1);
}

}  // namespace basic_topic

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<basic_topic::PublisherComponent>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
