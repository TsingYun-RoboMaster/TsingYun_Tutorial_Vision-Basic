#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion_topic", 10);
        
        // 创建定时器，每 1000 毫秒触发一次 timer_callback 函数
        timer_ = this->create_wall_timer(
            1000ms, std::bind(&PublisherComponent::timer_callback, this));
            
        RCLCPP_INFO(this->get_logger(), "Publisher Node Started.");
    }

    double PublisherComponent::normalize_angle(double angle)
    {
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    geometry_msgs::msg::Quaternion PublisherComponent::rpy_to_quaternion(double roll, double pitch, double yaw)
    {
        geometry_msgs::msg::Quaternion q;
        const double cy = std::cos(yaw * 0.5);
        const double sy = std::sin(yaw * 0.5);
        const double cp = std::cos(pitch * 0.5);
        const double sp = std::sin(pitch * 0.5);
        const double cr = std::cos(roll * 0.5);
        const double sr = std::sin(roll * 0.5);

        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }
        //todo
    void PublisherComponent::timer_callback()
    {
        // 1. 设定初始欧拉角。
        double base_roll = 1.0;
        double base_pitch = 0.5;
        double base_yaw = -1.0;

        double roll = base_roll + 2.0 * kPi;
        double pitch = base_pitch;
        double yaw = base_yaw - 2.0 * kPi;

        // 2. 打印发布前的欧拉角
        RCLCPP_INFO(this->get_logger(), 
            "Publishing RPY - Roll: %.4f, Pitch: %.4f, Yaw: %.4f", 
            roll, pitch, yaw);

        // 3. 调用提供的纯数学函数，将欧拉角转换为四元数
        geometry_msgs::msg::Quaternion q_msg = rpy_to_quaternion(roll, pitch, yaw);

        // 4. 将生成的四元数发布出去
        publisher_->publish(q_msg);
    }

    

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
