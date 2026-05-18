#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("/tf_quaternion", 10);

        timer_ = this->create_wall_timer(100ms, std::bind(&PublisherComponent::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "Publisher node started");
    }

    void PublisherComponent::timer_callback()
    {
        double time = this->now().seconds();
        double roll = 0.0;
        double pitch = 0.0;
        double yaw = time * 0.5; 

        auto q_msg = rpy_to_quaternion(roll, pitch, yaw);

        publisher_->publish(q_msg);

        RCLCPP_INFO(this->get_logger(), "Published RPY: [%.2f, %.2f, %.2f] rad", roll, pitch, yaw);
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

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
