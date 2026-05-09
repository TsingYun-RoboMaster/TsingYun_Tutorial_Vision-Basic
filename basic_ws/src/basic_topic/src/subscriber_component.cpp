#include "basic_topic/subscriber_component.hpp"

namespace basic_topic
{

    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        // TODO
        subscriber_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
            "/quaternion_topic", 10,
            std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "订阅节点(%s)已启动", this->get_name());
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
    void SubscriberComponent::topic_callback(const geometry_msgs::msg::Quaternion& msg)
    {
        double roll, pitch, yaw;
        quaternion_to_rpy(msg, roll, pitch, yaw);

        RCLCPP_INFO(this->get_logger(),
            "收到四元数: [%.2f, %.2f, %.2f, %.2f] | R: %.2f P: %.2f Y: %.2f",
            msg.x, msg.y, msg.z, msg.w,
            normalize_angle(roll), normalize_angle(pitch), normalize_angle(yaw));
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)
