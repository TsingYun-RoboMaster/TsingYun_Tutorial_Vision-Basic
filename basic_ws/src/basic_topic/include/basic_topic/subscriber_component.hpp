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
        // 圆周率常量用于角度归一化和 pitch 边界处理。
        static constexpr double kPi = 3.14159265358979323846;
        // 必须与 publisher_component 中的话题名保持一致。
        static constexpr const char* kTopicName = "/quaternion_topic";

        // 将恢复出的角度统一压到 [-pi, pi]，方便和发布端日志比较。
        double normalize_angle(double angle);
        // 把收到的四元数还原成 Roll/Pitch/Yaw 欧拉角。
        void quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q, double& roll, double& pitch, double& yaw);
        // ROS2 收到新消息时会自动调用这个回调函数。
        void on_quaternion(const geometry_msgs::msg::Quaternion::SharedPtr msg);

        // ROS2 subscription 句柄，负责监听 /quaternion_topic。
        rclcpp::Subscription<geometry_msgs::msg::Quaternion>::SharedPtr subscription_;
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__SUBSCRIBER_COMPONENT_HPP_
