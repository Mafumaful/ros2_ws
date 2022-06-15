#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  // 初始化
  rclcpp::init(argc, argv);

  // 创建一个节点，名称叫turtle_spawn_client
  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("turtle_spawn_client");
  // 创建一个客户端，订阅/spawn下的srv
  rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr client =
      node->create_client<turtlesim::srv::Spawn>("/spawn");

  // 创建一个请求，用来存放发出去的指令
  auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
  // 给指令赋值，name是海龟的名字，x是小海龟生成的x轴，y是小海龟生成的y轴
  request->name = "another_turtle";
  request->x = 2;
  request->y = 2;
  request->theta = 0;

  // 等待服务启动，若用户手动停止就报错，如果一直没有启动服务就每一秒循环一次
  while (!client->wait_for_service(1s))
  {
    if (!rclcpp::ok())
    {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }

  // 异步发送请求
  auto result = client->async_send_request(request);

  // 如果服务端在运行客户端节点以后生成，就等待服务完成
  if (rclcpp::spin_until_future_complete(node, result) ==
      rclcpp::executor::FutureReturnCode::SUCCESS)
  {
    // 打印回复结果
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "the new turtle's name is: %s", request.get()->name.c_str());
  }
  else
  {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Service call failed");
  }

  // 关闭节点
  rclcpp::shutdown();
  return 0;
}