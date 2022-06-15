// 包含头文件
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;

// 订阅速度节点，需要继承rclcpp::Node
class vel_Subscriber : public rclcpp::Node
{
public:
  // 构造函数
  vel_Subscriber();

private:
  // 回调函数
  void topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const;
  // 私有变量，订阅者，用于订阅速度消息
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
};

// 构造函数，指定节点名称，订阅者订阅话题名称
vel_Subscriber::vel_Subscriber() : Node("velocity_subscriber")
{
  subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "turtle1/cmd_vel", 10, std::bind(&vel_Subscriber::topic_callback, this, _1));
};

// 回调函数，用于发布订阅到的消息
void vel_Subscriber::topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const
{
  // 在终端打印订阅话题信息
  RCLCPP_INFO(this->get_logger(), "\r\n the speed of x is: '%f'\r\n the speed of y is: '%f' \r\n the speed of angular z is: '%f'\r\n",
              msg->linear.x, msg->linear.y, msg->angular.z);
}

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  // 使用rclcpp::spin创建my_c_subscriber_node
  rclcpp::spin(std::make_shared<vel_Subscriber>());
  rclcpp::shutdown();
  return 0;
}
