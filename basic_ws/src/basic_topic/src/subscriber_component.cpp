#include "basic_topic/subscriber_component.hpp"

namespace basic_topic
{

    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        subscription_ = create_subscription<geometry_msgs::msg::Quaternion>(
            "quaternion", 10,
            [this](const geometry_msgs::msg::Quaternion::SharedPtr msg) {
                double roll = 0.0;
                double pitch = 0.0;
                double yaw = 0.0;
                quaternion_to_rpy(*msg, roll, pitch, yaw);

                roll = normalize_angle(roll);
                pitch = normalize_angle(pitch);
                yaw = normalize_angle(yaw);

                RCLCPP_INFO(
                    get_logger(),
                    "Receive quaternion: x=%.6f y=%.6f z=%.6f w=%.6f | rpy: roll=%.6f pitch=%.6f yaw=%.6f",
                    msg->x, msg->y, msg->z, msg->w, roll, pitch, yaw);
            });
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

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)
