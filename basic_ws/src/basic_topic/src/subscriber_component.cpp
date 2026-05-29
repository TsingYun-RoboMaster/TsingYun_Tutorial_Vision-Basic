#include "basic_topic/subscriber_component.hpp"

namespace basic_topic
{

    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        // TODO
        subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
            "quaternion_topic", 10, std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));
            
        RCLCPP_INFO(this->get_logger(), "Subscriber Node Started. Waiting for messages...");
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

    // TODO
    void SubscriberComponent::topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg)
    {
        double roll, pitch, yaw;

        // 1. 调用自带的数学函数，将接收到的四元数解码为欧拉角
        quaternion_to_rpy(*msg, roll, pitch, yaw);

        // 2. 将解码后的角度进行归一化
        roll = normalize_angle(roll);
        pitch = normalize_angle(pitch);
        yaw = normalize_angle(yaw);

        // 3. 打印接收到的四元数以及 Roll, Pitch, Yaw
        RCLCPP_INFO(this->get_logger(), 
            "Received Quaternion: [x: %.2f, y: %.2f, z: %.2f, w: %.2f] | "
            "Decoded RPY -> Roll: %.4f, Pitch: %.4f, Yaw: %.4f", 
            msg->x, msg->y, msg->z, msg->w, roll, pitch, yaw);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)
