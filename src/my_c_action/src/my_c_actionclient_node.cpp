// 包含头文件
#include <functional>
#include <future>
#include <memory>
#include <string>

// 包含action文件
#include "my_action/action/count.hpp"

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rclcpp_components/register_node_macro.hpp"

// 设定命名空间my_c_actionclient_node
namespace my_c_actionclient_node
{
    // 新建一个类，名称为CountActionClient，继承rclcpp::Node节点
    class CountActionClient : public rclcpp::Node
    {
    public:
        // Count使用别名my_action::action::Count
        // GoalHandleCount使用别名rclcpp_action::ClientGoalHandle<Count>
        using Count = my_action::action::Count;
        using GoalHandleCount = rclcpp_action::ClientGoalHandle<Count>;

        // 构造函数
        CountActionClient(const rclcpp::NodeOptions &options)
            : Node("Count_action_client", options)
        {
            // 创建一个客户，绑定Count的服务名称
            this->client_ptr_ = rclcpp_action::create_client<Count>(
                this->get_node_base_interface(),
                this->get_node_graph_interface(),
                this->get_node_logging_interface(),
                this->get_node_waitables_interface(),
                "Count");

            // 利用句柄创建一个定时器,绑定回调函数sent_goal()
            this->timer_ = this->create_wall_timer(
                std::chrono::milliseconds(500),
                std::bind(&CountActionClient::send_goal, this));
        }

        // 发送目标
        void send_goal()
        {
            // 使用std::placeholders的命名空间
            using namespace std::placeholders;
            // 取消计时器
            this->timer_->cancel();

            // 等待服务器，准备好返回true或者超时就返回false
            if (!this->client_ptr_->wait_for_action_server())
            {
                // 打印错误日志
                RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
                rclcpp::shutdown();
            }

            // 创建一个Goal的实例
            auto goal_msg = Count::Goal();
            // 将目标赋值为10
            goal_msg.goal_num = 10;

            // 发送数据输出打印
            RCLCPP_INFO(this->get_logger(), "Sending goal");

            // 创建一个发送目标配置的实例
            auto send_goal_options = rclcpp_action::Client<Count>::SendGoalOptions();
            // 绑定目标应答的回调函数goal_response_callback
            send_goal_options.goal_response_callback =
                std::bind(&CountActionClient::goal_response_callback, this, _1);
            // 绑定目标反馈的回调函数feedback_callback
            send_goal_options.feedback_callback =
                std::bind(&CountActionClient::feedback_callback, this, _1, _2);
            // 绑定结果返回的回调函数result_callback
            send_goal_options.result_callback =
                std::bind(&CountActionClient::result_callback, this, _1);
            this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
        }

    private:
        // 创建一个客户
        rclcpp_action::Client<Count>::SharedPtr client_ptr_;
        // 创建定时器
        rclcpp::TimerBase::SharedPtr timer_;

        // 应答回调函数
        void goal_response_callback(std::shared_future<GoalHandleCount::SharedPtr> future)
        {
            // 获取应答数据
            auto goal_handle = future.get();
            if (!goal_handle)
            {
                // 打印被服务器拒绝的消息
                RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
            }
            else
            {
                // 打印服务器接收的消息
                RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
            }
        }

        // 反馈回调函数
        void feedback_callback(
            GoalHandleCount::SharedPtr,
            const std::shared_ptr<const Count::Feedback> feedback)
        {
            // 打印反馈回来的数据
            RCLCPP_INFO(this->get_logger(), "The progress is: %d%s", feedback->percent_complete, "%");
        }

        // 结果回调函数
        void result_callback(const GoalHandleCount::WrappedResult &result)
        {
            // switch语句判断结果
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

            // 打印结果回传数据
            RCLCPP_INFO(this->get_logger(), ss.str().c_str());
            rclcpp::shutdown();
        }
    };

}

// 使用组件创建节点
RCLCPP_COMPONENTS_REGISTER_NODE(my_c_actionclient_node::CountActionClient)