#include <functional>
#include <memory>
#include <thread>

#include "my_action/action/count.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rclcpp_components/register_node_macro.hpp"

namespace my_c_actionserver_node
{
  class CountActionServer : public rclcpp::Node
  {
  public:
    using Count = my_action::action::Count;
    using GoalHandleCount = rclcpp_action::ServerGoalHandle<Count>;

    CountActionServer(const rclcpp::NodeOptions &options = rclcpp::NodeOptions())
        : Node("Count_action_server", options)
    {
      using namespace std::placeholders;

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
    rclcpp_action::Server<Count>::SharedPtr action_server_;

    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &uuid,
        std::shared_ptr<const Count::Goal> goal)
    {
      RCLCPP_INFO(this->get_logger(), "Received goal, target is %d", goal->goal_num);
      (void)uuid;
      return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleCount> goal_handle)
    {
      RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
      (void)goal_handle;
      return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted(const std::shared_ptr<GoalHandleCount> goal_handle)
    {
      using namespace std::placeholders;
      // this needs to return quickly to avoid blocking the executor, so spin up a new thread
      std::thread{std::bind(&CountActionServer::execute, this, _1), goal_handle}.detach();
    }

    void execute(const std::shared_ptr<GoalHandleCount> goal_handle)
    {
      RCLCPP_INFO(this->get_logger(), "Executing goal");
      rclcpp::Rate loop_rate(10);
      const auto goal = goal_handle->get_goal();
      auto feedback = std::make_shared<Count::Feedback>();
      auto &rate_of_progress = feedback->percent_complete;
      auto result = std::make_shared<Count::Result>();

      for (int i = 1; (i <= goal->goal_num) && rclcpp::ok(); ++i)
      {
        // Check if there is a cancel request
        if (goal_handle->is_canceling())
        {
          result->finish = i;
          goal_handle->canceled(result);
          RCLCPP_INFO(this->get_logger(), "Goal canceled");
          return;
        }
        // Update rate_of_progress
        rate_of_progress = 100 * i / goal->goal_num;
        // Publish feedback
        goal_handle->publish_feedback(feedback);
        RCLCPP_INFO(this->get_logger(), "Publish feedback");

        loop_rate.sleep();
      }

      // Check if goal is done
      if (rclcpp::ok())
      {
        result->finish = goal->goal_num;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Goal succeeded");
      }
    }
  }; // class CountActionServer

} // namespace action_tutorials_cpp

RCLCPP_COMPONENTS_REGISTER_NODE(my_c_actionserver_node::CountActionServer)