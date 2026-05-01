#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&PublisherComponent::publish_quaternion, this));
        RCLCPP_INFO(this->get_logger(), "Publisher node started.");
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

    // TODO
    void PublisherComponent::publish_quaternion()
    {
        time_sec_ += 0.5;

        const double roll = normalize_angle(std::sin(time_sec_) * kPi);
        const double pitch = normalize_angle(std::cos(time_sec_ * 0.5) * kPi);
        const double yaw = normalize_angle(time_sec_);

        const auto q = rpy_to_quaternion(roll, pitch, yaw);

        publisher_->publish(q);

        RCLCPP_INFO(
            this->get_logger(),
            "Publish quaternion: x=%.4f y=%.4f z=%.4f w=%.4f | RPY: roll=%.4f pitch=%.4f yaw=%.4f",
            q.x,
            q.y,
            q.z,
            q.w,
            roll,
            pitch,
            yaw
        );
    }
}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
