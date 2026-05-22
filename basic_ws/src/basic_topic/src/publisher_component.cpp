#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO 
        
        // 1. 创建发布者：话题名字叫做 "topic_quaternion"，队列大小设置为 10
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("topic_quaternion", 10);

        // 2. 创建定时器：设置每 500 毫秒（0.5秒）执行一次 timer_callback 函数
        timer_ = this->create_wall_timer(500ms, std::bind(&PublisherComponent::timer_callback, this));
        
        RCLCPP_INFO(this->get_logger(), "Publisher Node has been initialized!");
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

    // ==== 【TODO 补全区域 2：编写定时器回调函数的具体逻辑】 ====
    void PublisherComponent::timer_callback()
    {
        // 1. 让角度每次定时器触发时自增一点（比如每次增加 0.1 弧度）
        roll_  += 0.1;
        pitch_ += 0.1;
        yaw_   += 0.1;

        // 2. 使用老师提供的 normalize_angle 函数将角度限制在 [-pi, pi] 之间
        double norm_roll  = normalize_angle(roll_);
        double norm_pitch = normalize_angle(pitch_);
        double norm_yaw   = normalize_angle(yaw_);

        // 3. 在终端打印当前的 Roll, Pitch, Yaw（对应作业要求 1.ii 打印对应的 RPY 值）
        RCLCPP_INFO(this->get_logger(), "Publishing RPY: [Roll: %.2f, Pitch: %.2f, Yaw: %.2f]", 
                    norm_roll, norm_pitch, norm_yaw);

        // 4. 调用转换公式，把欧拉角打包变成四元数 q
        geometry_msgs::msg::Quaternion q_msg = rpy_to_quaternion(norm_roll, norm_pitch, norm_yaw);

        // 5. 将四元数消息发布出去
        publisher_->publish(q_msg);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)