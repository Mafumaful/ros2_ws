// 包含头文件
#include <functional>
#include <future>
#include <memory>
#include <string>
#include <sstream>

#include "my_action/action/count.hpp"

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rclcpp_components/register_node_macro.hpp"

namespace my_c_actionclient_node
{
    class CountActionClient : public rclcpp::Node
    {
    public:
        using Count = my_action::action::Count;
        using GoalHandleCount = rclcpp_action::ClientGoalHandle<Count>;

        explicit CountActionClient(const rclcpp::NodeOptions &options)
            : Node("Count_action_client", options)
        {
            this->client_ptr_ = rclcpp_action::create_client<Count>(
                this->get_node_base_interface(),
                this->get_node_graph_interface(),
                this->get_node_logging_interface(),
                this->get_node_waitables_interface(),
                "Count");

            this->timer_ = this->create_wall_timer(
                std::chrono::milliseconds(500),
                std::bind(&CountActionClient::send_goal, this));
        }

        void send_goal()
        {
            using namespace std::placeholders;

            this->timer_->cancel();

            if (!this->client_ptr_->wait_for_action_server())
            {
                RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
                rclcpp::shutdown();
            }

            auto goal_msg = Count::Goal();
            goal_msg.goal_num = 10;

            RCLCPP_INFO(this->get_logger(), "Sending goal");

            auto send_goal_options = rclcpp_action::Client<Count>::SendGoalOptions();
            send_goal_options.goal_response_callback =
                std::bind(&CountActionClient::goal_response_callback, this, _1);
            send_goal_options.feedback_callback =
                std::bind(&CountActionClient::feedback_callback, this, _1, _2);
            send_goal_options.result_callback =
                std::bind(&CountActionClient::result_callback, this, _1);
            this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
        }

    private:
        rclcpp_action::Client<Count>::SharedPtr client_ptr_;
        rclcpp::TimerBase::SharedPtr timer_;

        void goal_response_callback(std::shared_future<GoalHandleCount::SharedPtr> future)
        {
            auto goal_handle = future.get();
            if (!goal_handle)
            {
                RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
            }
            else
            {
                RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
            }
        }

        void feedback_callback(
            GoalHandleCount::SharedPtr,
            const std::shared_ptr<const Count::Feedback> feedback)
        {
            RCLCPP_INFO(this->get_logger(), "The progress is: %d%s", feedback->percent_complete, "%");
        }

        void result_callback(const GoalHandleCount::WrappedResult &result)
        {
            switch (result.code)
            {
            case rclcpp_action::ResultCode::SUCCEEDED:
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(), "Goal was aborted");
                return;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
                return;
            default:
                RCLCPP_ERROR(this->get_logger(), "Unknown result code");
                return;
            }
            std::stringstream ss;
            ss << "Result received: ";
            ss << " " << result.result->finish;

            RCLCPP_INFO(this->get_logger(), ss.str().c_str());
            rclcpp::shutdown();
        }
    }; // class CountActionClient

} // namespace my_c_actionclient_node

RCLCPP_COMPONENTS_REGISTER_NODE(my_c_actionclient_node::CountActionClient)