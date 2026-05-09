#include "basic_topic/subscriber_component.hpp"

#include <functional>  // std::bind 用来把成员函数绑定成订阅回调

namespace basic_topic
{

    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        // 队列深度 10
        subscription_ = create_subscription<geometry_msgs::msg::Quaternion>(
            kTopicName,
            10,
            std::bind(&SubscriberComponent::on_quaternion, this, std::placeholders::_1));
        // 启动日志说明订阅端已经开始监听指定topic
        RCLCPP_INFO(get_logger(), "subscriber_component started, listening to %s", kTopicName);
    }

    double SubscriberComponent::normalize_angle(double angle)
    {
        // 与发布端使用同一套归一化逻辑，避免两个终端输出区间不同
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        // 修正负余数，让最终结果稳定落在 [-pi, pi]
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    void SubscriberComponent::quaternion_to_rpy(const geometry_msgs::msg::Quaternion& q, double& roll, double& pitch, double& yaw)
    {
        // roll = atan2(2(w x + y z), 1 - 2(x^2 + y^2))
        const double sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
        const double cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
        roll = std::atan2(sinr_cosp, cosr_cosp);

        // pitch = asin(2(w y - z x))，但数值误差可能让输入略微超过 [-1, 1]
        const double sinp = 2.0 * (q.w * q.y - q.z * q.x);
        // 超界时按正负号钳到 +/- pi/2，避免 asin 出现 NaN
        if (std::abs(sinp) >= 1.0)
            pitch = std::copysign(kPi / 2.0, sinp);
        else
            pitch = std::asin(sinp);

        // yaw = atan2(2(w z + x y), 1 - 2(y^2 + z^2))
        const double siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
        const double cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
        yaw = std::atan2(siny_cosp, cosy_cosp);
    }

    void SubscriberComponent::on_quaternion(const geometry_msgs::msg::Quaternion::SharedPtr msg)
    {
        // 先准备三个输出变量，quaternion_to_rpy 会通过引用写入结果
        double roll = 0.0;
        double pitch = 0.0;
        double yaw = 0.0;
        // 把收到的 ROS Quaternion 消息转换成更容易读懂的 RPY
        quaternion_to_rpy(*msg, roll, pitch, yaw);

        // 归一化到 [-pi, pi]，便于和发布端输出直接比较
        roll = normalize_angle(roll);
        pitch = normalize_angle(pitch);
        yaw = normalize_angle(yaw);

        // 打印原始四元数和转换后的 RPY
        RCLCPP_INFO(
            get_logger(),
            "receive quaternion: x=%.6f y=%.6f z=%.6f w=%.6f | RPY: roll=%.6f pitch=%.6f yaw=%.6f",
            msg->x,
            msg->y,
            msg->z,
            msg->w,
            roll,
            pitch,
            yaw);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)

int main(int argc, char* argv[])
{
    // 初始化 ROS2 客户端库，必须在创建节点之前调用
    rclcpp::init(argc, argv);
    // spin 会持续等待 topic 消息，并在消息到达时执行回调
    rclcpp::spin(std::make_shared<basic_topic::SubscriberComponent>());
    // 程序退出前释放 ROS2 相关资源
    rclcpp::shutdown();
    return 0;
}
