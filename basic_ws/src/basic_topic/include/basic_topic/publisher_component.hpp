#ifndef BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
#define BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <chrono>
#include <cmath>
#include <memory>
#include <string>

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

        // TODO
        void timer_callback();
        rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;

        double roll_;
        double pitch_;
        double yaw_;

    };

}  // namespace basic_topic

#endif  // BASIC_TOPIC__PUBLISHER_COMPONENT_HPP_
