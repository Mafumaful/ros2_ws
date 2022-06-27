// 包含头文件
#include <functional>
#include <memory>
#include <thread>

#include "my_action/action/count.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rclcpp_components/register_node_macro.hpp"

// 创建my_c_actionserver_node的命名空间
namespace my_c_actionserver_node
{
  // 新建一个类，名称为CountActionServer，继承rclcpp::Node节点
  class CountActionServer : public rclcpp::Node
  {
  public:
    // Count使用别名my_action::action::Count
    // GoalHandleCount使用别名rclcpp_action::ServerGoalHandle<Count>
    using Count = my_action::action::Count;
    using GoalHandleCount = rclcpp_action::ServerGoalHandle<Count>;

    // 构造函数
    CountActionServer(const rclcpp::NodeOptions &options = rclcpp::NodeOptions())
        : Node("Count_action_server", options)
    {
      // 使用std::placeholders命名空间
      using namespace std::placeholders;

      // 创建一个客户，创建Count的服务名称
      // 并绑定三个句柄handle_goal，handle_cancel，handle_accepted
      this->action_server_ = rclcpp_action::create_server<Count>(
          this->get_node_base_interface(),
          this->get_node_clock_interface(),
          this->get_node_logging_interface(),
          this->get_node_waitables_interface(),
          "Count",
          std::bind(&CountActionServer::handle_goal, this, _1, _2),
          std::bind(&CountActionServer::handle_cancel, this, _1),
          std::bind(&CountActionServer::handle_accepted, this, _1));
    }

  private:
    // 创建一个action服务器
    rclcpp_action::Server<Count>::SharedPtr action_server_;

    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &uuid,
        std::shared_ptr<const Count::Goal> goal)
    {
      // 打印目标信息
      RCLCPP_INFO(this->get_logger(), "Received goal, target is %d", goal->goal_num);
      // 静态检测，告诉静态代码检测工具程序并非没有处理该函数的返回值，而是确实不需要处理该函数的返回值，不需要再对该处代码作此项检测。
      (void)uuid;
      // 返回“接受并执行”的消息
      return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    // 如果目标被取消
    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleCount> goal_handle)
    {
      // 打印取消信息
      RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
      // 静态检测
      (void)goal_handle;
      // 返回“取消被接受”的消息
      return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted(const std::shared_ptr<GoalHandleCount> goal_handle)
    {
      // 使用std::placeholders命名空间
      using namespace std::placeholders;
      // 创建一个新的线程，函数名为execute，并开始执行execute()
      std::thread{std::bind(&CountActionServer::execute, this, _1), goal_handle}.detach();
    }

    void execute(const std::shared_ptr<GoalHandleCount> goal_handle)
    {
      // 打印正在执行的消息
      RCLCPP_INFO(this->get_logger(), "Executing goal");
      // 定义循环时间
      rclcpp::Rate loop_rate(10);
      // 通过goal_handle，获得目标值
      const auto goal = goal_handle->get_goal();
      // 创建feedback指针，指针类型为Count::Feedback
      auto feedback = std::make_shared<Count::Feedback>();
      // 定义feedback->percent_complete的别名为rate_of_progress，使之完全等价
      auto &rate_of_progress = feedback->percent_complete;
      // 创建result指针，指针类型为Count::Result
      auto result = std::make_shared<Count::Result>();

      for (int i = 1; (i <= goal->goal_num) && rclcpp::ok(); ++i)
      {
        // 检查请求有没有被取消
        if (goal_handle->is_canceling())
        {
          result->finish = i;
          goal_handle->canceled(result);
          RCLCPP_INFO(this->get_logger(), "Goal canceled");
          return;
        }
        // 更新进程
        rate_of_progress = 100 * i / goal->goal_num;
        // 发布反馈数据
        goal_handle->publish_feedback(feedback);
        RCLCPP_INFO(this->get_logger(), "Publish feedback");

        loop_rate.sleep();
      }

      // 检测目标是否已经完成
      if (rclcpp::ok())
      {
        result->finish = goal->goal_num;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Goal succeeded");
      }
    }
  };

}

// 使用组建创建节点
RCLCPP_COMPONENTS_REGISTER_NODE(my_c_actionserver_node::CountActionServer)