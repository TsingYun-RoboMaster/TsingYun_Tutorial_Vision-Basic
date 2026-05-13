#include "basic_topic/subscriber_component.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

namespace basic_topic
{
    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        // 订阅发布者的话题 "quaternion_topic"
        subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
            "quaternion_topic", 10, std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));
    }

    void SubscriberComponent::topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg)
    {
        // 1. 将消息中的四元数转换为 tf2 类型
        tf2::Quaternion q(msg->x, msg->y, msg->z, msg->w);

        // 2. 将四元数转换为旋转矩阵，进而提取欧拉角 (Roll, Pitch, Yaw)
        tf2::Matrix3x3 m(q);
        double r, p, y;
        m.getRPY(r, p, y);

        // 3. 按照作业要求：给 Yaw 增加一个 PI
        double modified_yaw = y + 3.14159265358979323846;

        // 4. 打印结果
        RCLCPP_INFO(this->get_logger(), 
            "Received RPY: [Roll: %.2f, Pitch: %.2f, Yaw: %.2f] -> Modified Yaw (y+pi): %.2f", 
            r, p, y, modified_yaw);
    }
}  // namespace basic_topic

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)