// 包含头文件
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;

class vel_Subscriber : public rclcpp::Node
{
public:
  vel_Subscriber()
      : Node("velocity_subscriber")
  {
    subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "turtle1/cmd_vel", 10, std::bind(&vel_Subscriber::topic_callback, this, _1));
  }

private:
  void topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const
  {
    RCLCPP_INFO(this->get_logger(), "\r\n the speed of x is: '%f'\r\n the speed of y is: '%f' \r\n the speed of angular z is: '%f'\r\n",
                msg->linear.x, msg->linear.y, msg->angular.z);
  }
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<vel_Subscriber>());
  rclcpp::shutdown();
  return 0;
}
