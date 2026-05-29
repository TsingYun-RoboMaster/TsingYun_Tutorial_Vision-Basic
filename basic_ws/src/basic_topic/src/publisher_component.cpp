#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // 创建发布者，话题名为 "quaternion"，队列深度 10
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion", 10);

        // 创建 500ms 周期的定时器，周期性发布四元数消息
        timer_ = this->create_wall_timer(500ms, std::bind(&PublisherComponent::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "Publisher node has been started.");
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
        // 生成 RPY。注意 quaternion_to_rpy 还原出的 pitch 只落在 [-pi/2, pi/2]，
        // 为保证四元数能被订阅端无歧义地还原（从而满足"两端相差 pi 整数倍"的要求），
        // 这里把 pitch 限制在 (-pi/2, pi/2)，roll、yaw 限制在 (-pi, pi)。
        const double roll = normalize_angle(roll_);
        const double pitch = 0.5 * normalize_angle(pitch_);  // 落在 (-pi/2, pi/2)
        const double yaw = normalize_angle(yaw_);

        // 转换为四元数并发布
        const geometry_msgs::msg::Quaternion msg = rpy_to_quaternion(roll, pitch, yaw);
        publisher_->publish(msg);

        // 在终端打印发布的 RPY 值
        RCLCPP_INFO(this->get_logger(),
                    "Publish  -> RPY: roll=%.4f, pitch=%.4f, yaw=%.4f", roll, pitch, yaw);

        // 更新 RPY，让下一次发布的内容发生变化
        roll_ += 0.1;
        pitch_ += 0.2;
        yaw_ += 0.3;
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
