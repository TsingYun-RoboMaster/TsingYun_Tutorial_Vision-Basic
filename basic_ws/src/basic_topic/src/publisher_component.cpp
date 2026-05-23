#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
// 1. 创建发布者对象
        // 指定消息泛型为 Quaternion，话题名称设定为 "basic_quaternion"，10 代表 QoS 队列深度（缓存区大小）
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("basic_quaternion", 10);

        // 2. 创建定时器对象
        // 设定触发周期为 500 毫秒。使用 std::bind 将类的成员函数 timer_callback 与当前对象实例 (this) 绑定作为回调目标
        timer_ = this->create_wall_timer(
            500ms, std::bind(&PublisherComponent::timer_callback, this));
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
        // 1. 更新欧拉角变量的值
        // 设定每次触发时按照固定步长递增，并调用类内方法约束在 [-π, π] 的主值区间内
        roll_ = normalize_angle(roll_ + 0.1);
        pitch_ = normalize_angle(pitch_ + 0.15);
        yaw_ = normalize_angle(yaw_ + 0.2);

        // 2. 终端打印日志
        // RCLCPP_INFO 是 ROS2 标准的日志输出宏，用法类似 C 语言的 printf
        RCLCPP_INFO(this->get_logger(), "Publishing -> Roll: %.2f, Pitch: %.2f, Yaw: %.2f", roll_, pitch_, yaw_);

        // 3. 数学解算与转换
        // 调用头文件中声明的 rpy_to_quaternion 方法，传入当前的欧拉角，返回对应的四元数消息对象
        auto msg = rpy_to_quaternion(roll_, pitch_, yaw_);

        // 4. 执行发布操作
        // 调用发布者对象的 publish 方法，将生成的四元数数据包推送到 DDS 通信网络中
        publisher_->publish(msg);
    }
}  // namespace basic_topic
int main(int argc, char * argv[])
{
    // 初始化 ROS 2 客户端库
    rclcpp::init(argc, argv);
    // 实例化你的发布者节点，并交由 spin 函数进入阻塞式的事件循环
    rclcpp::spin(std::make_shared<basic_topic::PublisherComponent>());
    // 节点被终止后，清理系统资源
    rclcpp::shutdown();
    return 0;
}
RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
