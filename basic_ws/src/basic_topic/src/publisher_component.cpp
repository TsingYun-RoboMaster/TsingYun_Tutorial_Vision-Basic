#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        publisher_ = create_publisher<geometry_msgs::msg::Quaternion>("quaternion", 10);

        timer_ = create_wall_timer(1s, [this]() {
            roll_ = normalize_angle(roll_ + 0.10);
            pitch_ = normalize_angle(pitch_ + 0.05);
            yaw_ = normalize_angle(yaw_ + 0.15);

            const auto msg = rpy_to_quaternion(roll_, pitch_, yaw_);
            publisher_->publish(msg);

            RCLCPP_INFO(
                get_logger(),
                "Publish quaternion: x=%.6f y=%.6f z=%.6f w=%.6f | rpy: roll=%.6f pitch=%.6f yaw=%.6f",
                msg.x, msg.y, msg.z, msg.w, roll_, pitch_, yaw_);
        });
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

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)

