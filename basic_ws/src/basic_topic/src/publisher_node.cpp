#include "basic_topic/publisher_component.hpp"

#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<basic_topic::PublisherComponent>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
