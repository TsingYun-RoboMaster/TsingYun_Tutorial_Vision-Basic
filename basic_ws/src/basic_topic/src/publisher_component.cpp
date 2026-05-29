#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO
        roll_ = 0.0;
        pitch_ = 0.0;
        yaw_ = 0.0;

        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>(
            "quaternion_topic",
            10);

        timer_ = this->create_wall_timer(
            1000ms,
            std::bind(&PublisherComponent::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "Publisher node started");
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
    void PublisherComponent::timer_callback()
    {
        roll_ += 0.1;
        pitch_ += 0.05;
        yaw_ += 0.08;

        roll_ = normalize_angle(roll_);
        pitch_ = normalize_angle(pitch_);
        yaw_ = normalize_angle(yaw_);

        geometry_msgs::msg::Quaternion msg =
            rpy_to_quaternion(roll_, pitch_, yaw_);

        publisher_->publish(msg);

        RCLCPP_INFO(
            this->get_logger(),
            "Publish RPY -> roll: %.3f, pitch: %.3f, yaw: %.3f",
            roll_,
            pitch_,
            yaw_);
}

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
