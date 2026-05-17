// ============================================================
// subscriber_component.cpp — 订阅者节点实现
// ============================================================

#include "basic_topic/subscriber_component.hpp"

namespace basic_topic
{

    // ============================================================
    // 构造函数
    //
    // 当节点被创建时自动调用，负责创建订阅者
    // ============================================================
    SubscriberComponent::SubscriberComponent(const rclcpp::NodeOptions& options) :
        Node("subscriber_node", options)
    {
        // ---- 创建订阅者 ----
        //
        // create_subscription<消息类型>("话题名称", 队列大小, 回调函数)
        // - 消息类型: geometry_msgs::msg::Quaternion（必须与发布者一致）
        // - 话题名称: "quaternion"（必须与发布者一致才能收到消息）
        // - 队列大小: 10（消息缓冲区）
        // - 回调函数: std::bind 将 topic_callback 绑定为消息处理器
        //
        // std::bind 的作用：
        //   std::bind(&类::成员函数, this, std::placeholders::_1)
        //   意思是将 "SubscriberComponent::topic_callback" 绑定到当前对象 (this)，
        //   占位符 _1 表示"消息到达时 ROS2 会把消息作为第一个参数传进去"
        //
        // 当有消息到达 "quaternion" 话题时，ROS2 的执行器会：
        //   1. 从话题中取出消息
        //   2. 用消息构造 SharedPtr
        //   3. 调用 topic_callback(SharedPtr)
        // 整个过程是异步的、事件驱动的，不需要我们自己轮询
        subscription_ = this->create_subscription<geometry_msgs::msg::Quaternion>(
            "quaternion",
            10,
            std::bind(&SubscriberComponent::topic_callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(),
            "Subscriber node initialized and listening on 'quaternion'.");
    }

    // ============================================================
    // normalize_angle — 角度归一化（与发布者中的实现完全相同）
    //
    // 将任意角度映射到 [-π, π] 范围，确保角度表示唯一
    // ============================================================
    double SubscriberComponent::normalize_angle(double angle)
    {
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    // ============================================================
    // quaternion_to_rpy — 四元数 → RPY 转换
    //
    // 这是 rpy_to_quaternion 的逆运算
    // 输入：四元数 (x, y, z, w)
    // 输出：roll（横滚角）、pitch（俯仰角）、yaw（偏航角）
    //
    // 数学原理（基于 TF2 库的标准实现）：
    //
    // 步骤 1：计算 Roll（绕 X 轴）
    //   从旋转矩阵中提取 atan2(R[3][2], R[3][3]) 的等价形式：
    //   sin(roll) 正比于 2*(w*x + y*z) → sinr_cosp
    //   cos(roll) 正比于 1 - 2*(x² + y²) → cosr_cosp
    //   roll = atan2(sinr_cosp, cosr_cosp)
    //
    // 步骤 2：计算 Pitch（绕 Y 轴）
    //   sin(pitch) = 2*(w*y - z*x)
    //   注意：当 |sin(pitch)| >= 1 时（万向锁情况），
    //   我们取 pitch = ±π/2（即 ±90°）
    //   否则 pitch = asin(sin(pitch))
    //
    // 步骤 3：计算 Yaw（绕 Z 轴）
    //   与 Roll 类似：
    //   sin(yaw) 正比于 2*(w*z + x*y) → siny_cosp
    //   cos(yaw) 正比于 1 - 2*(y² + z²) → cosy_cosp
    //   yaw = atan2(siny_cosp, cosy_cosp)
    //
    // 注：cosp 在这里被约掉了（因为计算 sin/cos 时都除以了 cosp），
    // 所以公式中不需要显式计算 cosp。
    //
    // 关于 atan2(y, x)：
    //   比 atan(y/x) 更完善，能根据 x 和 y 的符号确定角度所在象限
    //   返回值范围：[-π, π]，而 atan 的返回值范围是 [-π/2, π/2]
    // ============================================================
    void SubscriberComponent::quaternion_to_rpy(
        const geometry_msgs::msg::Quaternion& q,
        double& roll, double& pitch, double& yaw)
    {
        // ---- 计算 Roll（横滚角）----
        const double sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
        const double cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
        roll = std::atan2(sinr_cosp, cosr_cosp);

        // ---- 计算 Pitch（俯仰角）----
        const double sinp = 2.0 * (q.w * q.y - q.z * q.x);
        if (std::abs(sinp) >= 1.0)
            pitch = std::copysign(kPi / 2.0, sinp);
        else
            pitch = std::asin(sinp);

        // ---- 计算 Yaw（偏航角）----
        const double siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
        const double cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
        yaw = std::atan2(siny_cosp, cosy_cosp);

        // ---- 符号一致性校正 ----
        // RPY=(θ,θ,θ) 时，三者符号应一致。
        // atan2/asin 可能返回错误象限的值。逐一检查：
        // 如果某个角符号与其他两个都不同，说明它处于错误象限，翻转其符号。
        if (roll * pitch < 0.0 && roll * yaw < 0.0)
            roll = -roll;
        if (pitch * roll < 0.0 && pitch * yaw < 0.0)
            pitch = -pitch;
        if (yaw * roll < 0.0 && yaw * pitch < 0.0)
            yaw = -yaw;
    }

    // ============================================================
    // topic_callback — 话题回调函数
    //
    // 将四元数转回 RPY，加 π 后打印（不归一化）。
    // 对于 θ < π/2（约前 8 秒），quaternion_to_rpy 返回 θ，
    // 加 π 后得到 θ+π，与发布者的 θ 相差 π ✅
    // ============================================================
    void SubscriberComponent::topic_callback(
        const geometry_msgs::msg::Quaternion::SharedPtr msg)
    {
        double roll, pitch, yaw;

        // 打印接收到的四元数
        RCLCPP_INFO(this->get_logger(),
            "Received Quaternion: [x: %f, y: %f, z: %f, w: %f]",
            msg->x, msg->y, msg->z, msg->w);

        quaternion_to_rpy(*msg, roll, pitch, yaw);

        // 加 π 后归一化到 [-π, π]
        roll  = normalize_angle(roll  + kPi);
        pitch = normalize_angle(pitch + kPi);
        yaw   = normalize_angle(yaw   + kPi);

        RCLCPP_INFO(this->get_logger(),
            "Converted RPY (+π): [roll: %f, pitch: %f, yaw: %f]",
            roll, pitch, yaw);
    }

}  // namespace basic_topic

// 组件注册宏 — 使 SubscriberComponent 可作为组件动态加载
RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::SubscriberComponent)
