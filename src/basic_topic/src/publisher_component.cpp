#include "basic_topic/publisher_component.hpp"
#include <cmath>

using namespace std::chrono_literals;

namespace basic_topic
{
    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // 初始化发布者
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion_topic", 10);

        // 创建定时器
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

    void PublisherComponent::timer_callback()
    {
        // 设置作业要求的欧拉角 (例如 Roll=0, Pitch=0, Yaw=1.0)
        double r = 0.0, p = 0.0, y = 1.0;
        auto q_msg = rpy_to_quaternion(r, p, y);

        RCLCPP_INFO(this->get_logger(), "Publishing RPY: Roll=%.2f, Pitch=%.2f, Yaw=%.2f", r, p, y);
        publisher_->publish(q_msg);
    }

}  // namespace basic_topic (确保这行有大括号)

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)