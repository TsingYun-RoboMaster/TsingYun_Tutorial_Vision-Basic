#include "basic_topic/subscriber_component.hpp"

namespace basic_topic
{

    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        // 创建订阅者，订阅 "quaternion" 话题，队列深度 10
        subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
            "quaternion", 10,
            std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Subscriber node has been started.");
    }

    double SubscriberComponent::normalize_angle(double angle)
    {
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    void SubscriberComponent::quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q, double& roll, double& pitch, double& yaw)
    {
        const double sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
        const double cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
        roll = std::atan2(sinr_cosp, cosr_cosp);

        const double sinp = 2.0 * (q.w * q.y - q.z * q.x);
        if (std::abs(sinp) >= 1.0)
            pitch = std::copysign(kPi / 2.0, sinp);
        else
            pitch = std::asin(sinp);

        const double siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
        const double cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
        yaw = std::atan2(siny_cosp, cosy_cosp);
    }

    void SubscriberComponent::topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg)
    {
        // 将接收到的四元数转换为 RPY
        double roll = 0.0, pitch = 0.0, yaw = 0.0;
        quaternion_to_rpy(*msg, roll, pitch, yaw);

        // 作业要求：订阅端打印的 RPY 与发布端相差 pi 的整数倍。
        // 这里对每个角度加上 pi 后再归一化，使两端打印值恰好相差一个 pi。
        const double roll_shifted = normalize_angle(roll + kPi);
        const double pitch_shifted = normalize_angle(pitch + kPi);
        const double yaw_shifted = normalize_angle(yaw + kPi);

        // 打印接收到的四元数
        RCLCPP_INFO(this->get_logger(),
                    "Received -> Quaternion: x=%.4f, y=%.4f, z=%.4f, w=%.4f",
                    msg->x, msg->y, msg->z, msg->w);
        // 打印转换并偏移 pi 后的 RPY
        RCLCPP_INFO(this->get_logger(),
                    "Received -> RPY (shifted by pi): roll=%.4f, pitch=%.4f, yaw=%.4f",
                    roll_shifted, pitch_shifted, yaw_shifted);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)
