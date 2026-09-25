#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

class PinSubscriber : public rclcpp::Node {
  public:
    PinSubscriber() : Node("pin_subscriber") {
      auto topic_callback =
        [this](std_msgs::msg::Bool::UniquePtr msg) -> void {
          RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data ? "true" : "false");
        };
      subscription_ = this->create_subscription<std_msgs::msg::Bool>("pin", 10, topic_callback);
    }

  private:
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription_;
};

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PinSubscriber>());
  rclcpp::shutdown();
  return 0;
}
