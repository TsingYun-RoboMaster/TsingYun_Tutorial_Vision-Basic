#include "basic_topic/publisher_component.hpp"

#include <functional>

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        publisher_ = create_publisher<geometry_msgs::msg::Quaternion>("quaternion_topic", 10);
        start_time_ = now();
        timer_ = create_wall_timer(1s, std::bind(&PublisherComponent::timer_callback, this));
    }

    double PublisherComponent::normalize_angle(double angle)
    {
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    geometry_msgs::msg::Quaternion PublisherComponent::rpy_to_quaternion(double roll, double pitch, double yaw)
    {
        geometry_msgs::msg::Quaternion q;
        const double cy = std::cos(yaw * 0.5);
        const double sy = std::sin(yaw * 0.5);
        const double cp = std::cos(pitch * 0.5);
        const double sp = std::sin(pitch * 0.5);
        const double cr = std::cos(roll * 0.5);
        const double sr = std::sin(roll * 0.5);

        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }

    void PublisherComponent::timer_callback()
    {
        const double elapsed = (now() - start_time_).seconds();
        const double roll = normalize_angle(elapsed * 0.1);
        const double pitch = normalize_angle(elapsed * 0.15);
        const double yaw = normalize_angle(elapsed * 0.2);

        const auto quaternion = rpy_to_quaternion(roll, pitch, yaw);
        publisher_->publish(quaternion);

        RCLCPP_INFO(
            get_logger(),
            "Published Quaternion [x: %.4f, y: %.4f, z: %.4f, w: %.4f] | Roll: %.4f, Pitch: %.4f, Yaw: %.4f",
            quaternion.x,
            quaternion.y,
            quaternion.z,
            quaternion.w,
            roll,
            pitch,
            yaw);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
