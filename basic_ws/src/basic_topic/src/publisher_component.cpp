#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO
        pub_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion_topic", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&PublisherComponent::timer_callback, this));

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
    double PublisherComponent::randomDouble(double min, double max)
    {
        static std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }
    
    void PublisherComponent::timer_callback()
    {
        auto message = geometry_msgs::msg::Quaternion();
        double roll  = randomDouble(-kPi, kPi);
        double pitch = randomDouble(-kPi, kPi);
        double yaw   = randomDouble(-kPi, kPi);
        message = rpy_to_quaternion(roll, pitch, yaw);
        RCLCPP_INFO(this->get_logger(), "Publishing: [w: %.3f, x: %.3f, y: %.3f, z: %.3f] from [%.3f, %.3f, %.3f]", 
                message.w, message.x, message.y, message.z, roll, pitch, yaw);
        pub_->publish(message);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
