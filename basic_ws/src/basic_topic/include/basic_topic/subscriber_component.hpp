#ifndef BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
#define BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <cmath>
#include <memory>
#include <string>

namespace basic_topic
{

    class SubscriberComponent : public rclcpp::Node
    {
    public:
        explicit SubscriberComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        static constexpr double kPi = 3.14159265358979323846;
        double normalize_angle(double angle);
        void quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q, double& roll, double& pitch, double& yaw);
// 声明 ROS 2 订阅者智能指针
        rclcpp::Subscription<geometry_msgs::msg::Quaternion>::SharedPtr subscription_;
        
        // 声明接收到消息时触发的回调函数，参数类型必须与发布的话题类型严格对应
        void topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg);
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
