#include "basic_topic/publisher_component.hpp"

#include <functional>  // std::bind 用来把成员函数绑定给 ROS2 定时器。

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options), sample_index_(0)
    {
        // ROS2 最多缓存 10 条消息
        publisher_ = create_publisher<geometry_msgs::msg::Quaternion>(kTopicName, 10);

        // 每 1 秒发布一次，方便在终端和录屏中清楚看到两端通信
        timer_ = create_wall_timer(1s, std::bind(&PublisherComponent::publish_quaternion, this));
        // 启动日志说明当前节点已经创建 publisher，并标出实际话题名
        RCLCPP_INFO(get_logger(), "publisher_component started, publishing to %s", kTopicName);
    }

    double PublisherComponent::normalize_angle(double angle)
    {
        // 先整体平移 pi，再对 2pi 取模，最后平移回 [-pi, pi] 区间
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        // fmod 对负数可能返回负余数，这里补一个 2pi 保证结果落在正周期内
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    geometry_msgs::msg::Quaternion PublisherComponent::rpy_to_quaternion(double roll, double pitch, double yaw)
    {
        geometry_msgs::msg::Quaternion q;
        // 四元数公式使用半角三角函数，cy/sy 对应 yaw 的 cos/sin 半角
        const double cy = std::cos(yaw * 0.5);
        const double sy = std::sin(yaw * 0.5);
        // cp/sp 对应 pitch 的 cos/sin 半角
        const double cp = std::cos(pitch * 0.5);
        const double sp = std::sin(pitch * 0.5);
        // cr/sr 对应 roll 的 cos/sin 半角
        const double cr = std::cos(roll * 0.5);
        const double sr = std::sin(roll * 0.5);

        // 这里采用常见的 ZYX 顺序：yaw -> pitch -> roll，对应 ROS 中常用的 RPY 表达
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }

    void PublisherComponent::publish_quaternion()
    {
        // sample_index_ 每次递增，使发布值持续变化，但每次运行的序列一致
        const double t = static_cast<double>(sample_index_++);
        // roll 和 yaw 做归一化，防止长时间运行后角度无限增大
        const double roll = normalize_angle(0.25 * t);
        // pitch 用较小幅值的 sin，避免接近 +/- pi/2 时欧拉角表示不稳定
        const double pitch = 0.35 * std::sin(0.4 * t);
        const double yaw = normalize_angle(0.40 * t);
        // 只在线上传 Quaternion，RPY 仅用于日志显示和对照检查
        const auto q = rpy_to_quaternion(roll, pitch, yaw);

        // 把消息发到 ROS2 topic，订阅节点会从同一个 topic 收到它
        publisher_->publish(q);
        // 打印 quaternion 和原始 RPY，便于和订阅端恢复出的 RPY 对比
        RCLCPP_INFO(
            get_logger(),
            "publish quaternion: x=%.6f y=%.6f z=%.6f w=%.6f | RPY: roll=%.6f pitch=%.6f yaw=%.6f",
            q.x,
            q.y,
            q.z,
            q.w,
            roll,
            pitch,
            yaw);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)

int main(int argc, char* argv[])
{
    // 初始化 ROS2 客户端库，必须在创建节点之前调用
    rclcpp::init(argc, argv);
    // spin 会一直处理定时器回调，让节点持续发布消息
    rclcpp::spin(std::make_shared<basic_topic::PublisherComponent>());
    // 程序退出前释放 ROS2 相关资源
    rclcpp::shutdown();
    return 0;
}
