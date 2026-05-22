#include "basic_topic/subscriber_component.hpp"

namespace basic_topic
{

    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        // TODO 1
        // 创建订阅者：必须去监听和发布者一模一样的话题 "topic_quaternion"，队列大小设置为 10
        subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
            "topic_quaternion", 10, std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Subscriber Node has been initialized!");
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

    // TODO 2
    void SubscriberComponent::topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg)
    {
        double rx_roll = 0.0;
        double rx_pitch = 0.0;
        double rx_yaw = 0.0;

        // 1. 调用老师给的函数，把收到的四元数解算回原始欧拉角
        quaternion_to_rpy(*msg, rx_roll, rx_pitch, rx_yaw);

        // 2. 满足作业要求 1.iv：让接收端的 RPY 与发送端相差 pi 的整数倍
        // 我们给解算出的角度直接加上一个 pi
        double target_roll  = rx_roll  + kPi;
        double target_pitch = rx_pitch + kPi;
        double target_yaw   = rx_yaw   + kPi;

        // 3. 顺手用老师写好的 normalize_angle 函数，把加了 pi 之后的角度规整到 [-pi, pi] 之间
        target_roll  = normalize_angle(target_roll);
        target_pitch = normalize_angle(target_pitch);
        target_yaw   = normalize_angle(target_yaw);

        // 4. 满足作业要求 1.iii：在终端打印出收到的四元数与转换后的 RPY 值
        RCLCPP_INFO(this->get_logger(), "----------------------------------------");
        RCLCPP_INFO(this->get_logger(), "Received Quaternion: [w: %.2f, x: %.2f, y: %.2f, z: %.2f]", 
                    msg->w, msg->x, msg->y, msg->z);
        RCLCPP_INFO(this->get_logger(), "Calculated RPY (with pi offset): [Roll: %.2f, Pitch: %.2f, Yaw: %.2f]", 
                    target_roll, target_pitch, target_yaw);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)