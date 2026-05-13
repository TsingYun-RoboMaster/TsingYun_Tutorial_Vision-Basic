#ifndef BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
#define BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

namespace basic_topic
{
    class SubscriberComponent : public rclcpp::Node
    {
    public:
        explicit SubscriberComponent(const rclcpp::NodeOptions & options);

    private:
        // --- 核心修复：添加回调函数声明和订阅者对象 ---
        void topic_callback(const geometry_msgs::msg::Quaternion::SharedPtr msg);
        rclcpp::Subscription<geometry_msgs::msg::Quaternion>::SharedPtr subscription_;
        // ------------------------------------------
    };
}  // namespace basic_topic

#endif  // BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_