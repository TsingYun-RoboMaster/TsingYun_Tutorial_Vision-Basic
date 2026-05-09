#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // TODO
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>(
            "/quaternion_topic", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&PublisherComponent::timer_callback, this));
        RCLCPP_INFO(this->get_logger(), "发布节点(%s)已启动", this->get_name());
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

    // TODO
    void PublisherComponent::timer_callback()
    {
        // 随机生成欧拉角（范围 -π ~ π）
        double roll = (static_cast<double>(std::rand()) / RAND_MAX) * 2.0 * kPi - kPi;
        double pitch = (static_cast<double>(std::rand()) / RAND_MAX) * 2.0 * kPi - kPi;
        double yaw = (static_cast<double>(std::rand()) / RAND_MAX) * 2.0 * kPi - kPi;

        // 欧拉角转四元数
        geometry_msgs::msg::Quaternion msg = rpy_to_quaternion(roll, pitch, yaw);

        // 发布消息
        publisher_->publish(msg);

        // 打印日志
        RCLCPP_INFO(this->get_logger(),
            "发布四元数: [%.2f, %.2f, %.2f, %.2f] | R: %.2f P: %.2f Y: %.2f",
            msg.x, msg.y, msg.z, msg.w,
            normalize_angle(roll), normalize_angle(pitch), normalize_angle(yaw));
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
