#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class TestCallbackGroup : public rclcpp::Node
{
public:
  TestCallbackGroup()
  : Node("Hermit")
  {
    // 选择 MutuallyExclusive 或者 Reentrant 线程池的回调类型,这里我们选择并行的形式
    //my_callback_group = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    my_callback_group = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

    auto sub_opt = rclcpp::SubscriptionOptions();
    sub_opt.callback_group = my_callback_group;

    // // Run timer at 10fps
    timer_ = this->create_wall_timer(
      100ms, std::bind(&TestCallbackGroup::timer_callback, this), my_callback_group);

    // // Run subscription with depth 10
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "/sub1", 10, std::bind(&TestCallbackGroup::topic_callback, this, std::placeholders::_1),sub_opt);
  }

private:
  void timer_callback()
  {
    std::cout << "timer callback" << std::endl;
  }

  void topic_callback(const std_msgs::msg::String::SharedPtr msg)
  {
    std::cout << "subscription callback" << std::endl;
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::CallbackGroup::SharedPtr my_callback_group;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<TestCallbackGroup>();

  // 选择 SingleThreadedExecutor 或者 MultiThreadedExecutor
  // rclcpp::executors::SingleThreadedExecutor executor;
  rclcpp::executors::MultiThreadedExecutor executor;

  // 多进程添加节点
  executor.add_node(node);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
