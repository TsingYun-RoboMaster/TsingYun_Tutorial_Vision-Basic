#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO: 创建发布者和定时器
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion_topic", 10);
        timer_ = this->create_wall_timer(500ms, [this]() { timer_callback(); });
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

    // TODO: 定时器回调，生成 RPY 并发布四元数
    void PublisherComponent::timer_callback()
    {
        double roll = 1.0;
        double pitch = 0.5;
        double yaw = 0.3;

        auto msg = rpy_to_quaternion(roll, pitch, yaw);
        publisher_->publish(msg);

        RCLCPP_INFO(this->get_logger(), "Publishing - Roll: %.4f, Pitch: %.4f, Yaw: %.4f",
                    roll, pitch, yaw);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
