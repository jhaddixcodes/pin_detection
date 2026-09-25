#include <chrono>
#include <memory>
#include <string>
#include <gpiod.hpp> // for interfacing with GPIO pins

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

using namespace std::chrono_literals;

class PinPublisher : public rclcpp::Node {
  public:
    PinPublisher() : Node("pin_publisher"), chip_("gpiochip0") {
      publisher_ = this->create_publisher<std_msgs::msg::Bool>(
        "pin", 10
      );

      out_line_ = chip_.get_line(17);
      out_line_.request({
        "ros2_publisher",
        gpiod::line_request::DIRECTION_OUTPUT,
        0
      });

      in_line_ = chip_.get_line(26);
      in_line_.request({
        "ros2_publisher",
        gpiod::line_request::DIRECTION_INPUT,
        0
      });

      out_line_.set_value(1);

      auto timer_callback = [this]() -> void {
        auto message = std_msgs::msg::Bool();
        message.data = in_line_.get_value() == 1;
        RCLCPP_INFO(this->get_logger(), "Publishing: %s", message.data ? "true" : "false");
        this->publisher_->publish(message);
      };
      timer_ = this->create_wall_timer(2000ms, timer_callback);
    };
  private:
    rclcpp::TimerBase::SharedPtr                      timer_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_;
    gpiod::chip                                       chip_;
    gpiod::line                                       out_line_;
    gpiod::line                                       in_line_;
};

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PinPublisher>());
  rclcpp::shutdown();
  return 0;
};
