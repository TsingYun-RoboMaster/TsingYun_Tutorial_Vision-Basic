#ifndef BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
#define BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp" //ROS 2 的 C++ 客户端库。提供 Node 类、Publisher 类、Timer 类等
#include "rclcpp_components/register_node_macro.hpp" //提供 RCLCPP_COMPONENTS_REGISTER_NODE 宏，让节点可以作为组件被动态加载。
#include "geometry_msgs/msg/quaternion.hpp" //ROS 2 中用于表示四元数的消息类型，包含 x、y、z、w 四个成员变量。

#include <chrono> //提供 std::chrono 库，用于处理时间相关的功能，如定时器。
#include <cmath> //提供数学函数，如 std::sin、std::cos、std::atan2 等，用于角度转换和计算。
#include <memory> //提供智能指针，如 std::shared_ptr 和 std::unique_ptr，用于管理动态分配的内存。
#include <string> //提供 std::string 类，用于处理字符串数据。

namespace basic_topic
{

    class PublisherComponent : public rclcpp::Node
    {
    public:
        explicit PublisherComponent(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

    private:
        static constexpr double kPi = 3.14159265358979323846;
        double normalize_angle(double angle);
        geometry_msgs::msg::Quaternion rpy_to_quaternion(double roll, double pitch, double yaw);

        //TODO
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        void timer_callback();
    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
