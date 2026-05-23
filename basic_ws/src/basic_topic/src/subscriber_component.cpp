#include "basic_topic/subscriber_component.hpp"

namespace basic_topic
{

    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
    // 创建订阅者对象
        // 指定消息泛型为 Quaternion，监听名称为 "basic_quaternion" 的话题，QoS 队列深度设为 10
        // std::placeholders::_1 是占位符，表示当底层收到消息时，将消息数据作为第一个参数传递给回调函数
        subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
            "basic_quaternion", 10, std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));
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
        double roll, pitch, yaw;
        
        // 1. 数据解算：传入接收到的四元数消息对象（解除智能指针的解引用 *msg），获取对应的欧拉角
        quaternion_to_rpy(*msg, roll, pitch, yaw);

        // 2. 终端打印：按照作业要求，同时打印接收到的四元数原始值和解算出的 RPY 值
        RCLCPP_INFO(this->get_logger(), 
            "Received Quaternion [w: %.2f, x: %.2f, y: %.2f, z: %.2f]", 
            msg->w, msg->x, msg->y, msg->z);
        RCLCPP_INFO(this->get_logger(), 
            "Parsed RPY -> Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", 
            roll, pitch, yaw);
    }

}  // namespace basic_topic
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<basic_topic::SubscriberComponent>());
    rclcpp::shutdown();
    return 0;
}
RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)
