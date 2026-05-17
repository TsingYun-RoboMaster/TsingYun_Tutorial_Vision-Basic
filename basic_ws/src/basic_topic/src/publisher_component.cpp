// ============================================================
// publisher_component.cpp — 发布者节点实现
// ============================================================

#include "basic_topic/publisher_component.hpp"

// 使用标准库中的时间字面量，例如 500ms 表示 500 毫秒
// 这让我们可以写 this->create_wall_timer(500ms, ...) 而不是
// this->create_wall_timer(std::chrono::milliseconds(500), ...)
using namespace std::chrono_literals;

namespace basic_topic
{

    // ============================================================
    // 构造函数
    //
    // 当节点被创建时自动调用，负责初始化发布者和定时器
    //
    // :param options: NodeOptions 对象，用于配置节点（组件加载时传入）
    //
    // ": Node("publisher_node", options)" 是 C++ 初始化列表，
    // 调用基类 rclcpp::Node 的构造函数，将节点命名为 "publisher_node"
    // ============================================================
    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        // ---- 1. 创建发布者 ----
        //
        // create_publisher<消息类型>("话题名称", 队列大小)
        // - 消息类型: geometry_msgs::msg::Quaternion
        // - 话题名称: "quaternion"（订阅者必须使用相同的话题名才能通信）
        // - 队列大小: 10（消息缓冲区最多存 10 条，超过则丢弃最旧的）
        //
        // 返回值是一个 SharedPtr（共享指针），ROS2 中所有核心对象都用它管理
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion", 10);

        // ---- 2. 创建定时器 ----
        //
        // create_wall_timer(时间间隔, 回调函数)
        // - 时间间隔: 500ms（每 0.5 秒触发一次）
        // - 回调函数: 一个 lambda 表达式 [this](){ ... }
        //
        // 定时器触发时，ROS2 的 Executor（执行器）会自动调用这个 lambda
        // 我们不需要自己写循环，ROS2 会在后台处理这一切
        timer_ = this->create_wall_timer(500ms, [this]() {
            // ---- 直接生成四元数（通过四元数乘法递增）----
            // 静态四元数，每次迭代乘一个小旋转增量
            static geometry_msgs::msg::Quaternion q;
            static bool first = true;
            if (first) {
                q.x = 0.0; q.y = 0.0; q.z = 0.0; q.w = 1.0;
                first = false;
            }

            // 每次旋转 0.1 rad，绕轴 (1,1,1)/√3（等量改变 roll/pitch/yaw）
            constexpr double inv_sqrt3 = 0.5773502691896258;
            constexpr double theta_step = 0.1;
            const double c = std::cos(theta_step / 2.0);
            const double s = std::sin(theta_step / 2.0);
            // 增量四元数
            const double dw = c;
            const double dx = inv_sqrt3 * s;
            const double dy = inv_sqrt3 * s;
            const double dz = inv_sqrt3 * s;

            // 四元数乘法: q_new = q * delta_q
            auto quaternion_msg = geometry_msgs::msg::Quaternion();
            quaternion_msg.w = q.w * dw - q.x * dx - q.y * dy - q.z * dz;
            quaternion_msg.x = q.w * dx + q.x * dw + q.y * dz - q.z * dy;
            quaternion_msg.y = q.w * dy - q.x * dz + q.y * dw + q.z * dx;
            quaternion_msg.z = q.w * dz + q.x * dy - q.y * dx + q.z * dw;

            // 保存当前四元数供下次使用
            q = quaternion_msg;

            // ---- 将四元数转为 RPY 并打印 ----
            const auto& qr = quaternion_msg;
            double d_roll, d_pitch, d_yaw;
            {
                const double sr = 2.0 * (qr.w * qr.x + qr.y * qr.z);
                const double cr = 1.0 - 2.0 * (qr.x * qr.x + qr.y * qr.y);
                d_roll = std::atan2(sr, cr);
                const double sp = 2.0 * (qr.w * qr.y - qr.z * qr.x);
                d_pitch = (std::abs(sp) >= 1.0)
                    ? std::copysign(kPi / 2.0, sp) : std::asin(sp);
                const double sy = 2.0 * (qr.w * qr.z + qr.x * qr.y);
                const double cy = 1.0 - 2.0 * (qr.y * qr.y + qr.z * qr.z);
                d_yaw = std::atan2(sy, cy);
                // 符号校正
                if (d_roll * d_pitch < 0.0 && d_roll * d_yaw < 0.0)
                    d_roll = -d_roll;
                if (d_pitch * d_roll < 0.0 && d_pitch * d_yaw < 0.0)
                    d_pitch = -d_pitch;
                if (d_yaw * d_roll < 0.0 && d_yaw * d_pitch < 0.0)
                    d_yaw = -d_yaw;
            }

            RCLCPP_INFO(this->get_logger(),
                "Publishing RPY: [roll: %f, pitch: %f, yaw: %f]",
                d_roll, d_pitch, d_yaw);

            // 发布四元数
            publisher_->publish(quaternion_msg);
        });
    }

    // ============================================================
    // normalize_angle — 角度归一化
    //
    // 功能：将任意角度（单位：弧度）映射到 [-π, π] 范围内
    //
    // 为什么要归一化？
    // 角度可以有无数种表示方式，例如：
    //   0 rad = 2π rad = 4π rad = ...（同一个物理角度）
    //   3π/2 rad = -π/2 rad（同一个方向）
    // 归一化后，每个角度只有唯一的表示，便于比较和计算
    //
    // 算法原理（三步）：
    //   1. angle + π  →  将范围平移，使 0 变为 π
    //   2. fmod(..., 2π)  →  取模 2π，将角度映射到 [0, 2π) 范围
    //   3. - π  →  平移回去，得到 [-π, π] 范围
    //
    // 举例：angle = 450° = 2.5π rad
    //   1. 2.5π + π = 3.5π
    //   2. fmod(3.5π, 2π) = 1.5π
    //   3. 1.5π - π = 0.5π（即 90°）✓
    // ============================================================
    double PublisherComponent::normalize_angle(double angle)
    {
        // fmod(a, b) = a - b * floor(a / b)，即浮点数取余运算
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    // ============================================================
    // rpy_to_quaternion — RPY → 四元数 转换
    //
    // 输入：roll（横滚，绕 X 轴）, pitch（俯仰，绕 Y 轴）, yaw（偏航，绕 Z 轴）
    // 输出：一个单位四元数（长度为 1 的四元数）
    //
    // 数学原理：
    //   三维空间中的任意旋转可以分解为三个基本旋转的复合：
    //     1. 绕 Z 轴旋转 yaw（偏航）  → 矩阵 Rz
    //     2. 绕 Y 轴旋转 pitch（俯仰）→ 矩阵 Ry
    //     3. 绕 X 轴旋转 roll（横滚） → 矩阵 Rx
    //
    //   总旋转矩阵 R = Rz * Ry * Rx（ZYX 顺序，这是航空领域的标准约定）
    //
    //   每个基本旋转可以用一个四元数表示：
    //     Qyaw   = (cos(y/2), 0, 0, sin(y/2))          绕 Z 轴
    //     Qpitch = (cos(p/2), 0, sin(p/2), 0)          绕 Y 轴
    //     Qroll  = (cos(r/2), sin(r/2), 0, 0)          绕 X 轴
    //
    //   复合旋转的四元数 = Qyaw * Qpitch * Qroll（四元数乘法）
    //   展开后得到下面的四个公式。其中：
    //     cr = cos(roll/2),  sr = sin(roll/2)
    //     cp = cos(pitch/2), sp = sin(pitch/2)
    //     cy = cos(yaw/2),   sy = sin(yaw/2)
    //
    //   结果：
    //     w = cr*cp*cy + sr*sp*sy
    //     x = sr*cp*cy - cr*sp*sy
    //     y = cr*sp*cy + sr*cp*sy
    //     z = cr*cp*sy - sr*sp*cy
    //
    //   验证：当 roll=pitch=yaw=0 时，四元数为 (1, 0, 0, 0)，表示无旋转 ✓
    // ============================================================
    geometry_msgs::msg::Quaternion PublisherComponent::rpy_to_quaternion(
        double roll, double pitch, double yaw)
    {
        geometry_msgs::msg::Quaternion q;

        // 计算各半角的三角函数值（只需计算一次）
        const double cy = std::cos(yaw * 0.5);    // cos(yaw/2)
        const double sy = std::sin(yaw * 0.5);    // sin(yaw/2)
        const double cp = std::cos(pitch * 0.5);  // cos(pitch/2)
        const double sp = std::sin(pitch * 0.5);  // sin(pitch/2)
        const double cr = std::cos(roll * 0.5);   // cos(roll/2)
        const double sr = std::sin(roll * 0.5);   // sin(roll/2)

        // 四元数复合公式（ZYX 顺序）
        q.w = cr * cp * cy + sr * sp * sy;  // 实部（标量部分）
        q.x = sr * cp * cy - cr * sp * sy;  // 虚部 i（绕 X 轴）
        q.y = cr * sp * cy + sr * cp * sy;  // 虚部 j（绕 Y 轴）
        q.z = cr * cp * sy - sr * sp * cy;  // 虚部 k（绕 Z 轴）

        return q;
    }

}  // namespace basic_topic

// ============================================================
// 组件注册宏
//
// 这行代码是 rclcpp_components 机制的关键：
// 它将 PublisherComponent 类注册为一个可动态加载的 ROS2 组件。
//
// 组件（Component）是一种特殊的 ROS2 节点，特点：
// 1. 编译为共享库（.so 文件），而不是独立可执行文件
// 2. 可以在运行时被动态加载到 ComponentContainer 中
// 3. 多个组件可以放在同一个进程/线程中，减少通信开销
//
// 共享库 + 独立可执行文件两种方式都支持（CMakeLists.txt 中分别配置）
// ============================================================
RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
