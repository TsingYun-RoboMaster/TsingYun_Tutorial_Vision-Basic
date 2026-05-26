#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>

class PublisherNode : public rclcpp::Node {
public:
  PublisherNode() : Node("publisher_node") {
    publisher_ = this->create_publisher<std_msgs::msg::String>("chat_topic", 10);
    timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&PublisherNode::timer_callback, this));
  }
private:
  void timer_callback() {
    auto message = std_msgs::msg::String();
    message.data = "李俊泽 - 2025013550: Hello ROS 2!";
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PublisherNode>());
  rclcpp::shutdown();
  return 0;
}
