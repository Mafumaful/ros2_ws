// 包含头文件
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

namespace my_composition
{
    // 发布速度的节点，需要继承rclcpp::Node
    class my_c_publisher_node : public rclcpp::Node
    {
    private:
        float vel_gain = 0; // 速度成员变量
        void vel_cb();      // 回调函数，用于发布消息
        // 构建速度发布者，其消息类型为geometry_msgs::msg::Twist
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub_;
        // 定时器创建
        rclcpp::TimerBase::SharedPtr timer_;

    public:
        // 构造函数
        my_c_publisher_node(const rclcpp::NodeOptions &options);
    };

    // 创建构造函数的时候，需要为节点命名，在这里名称为“vel_publilsher”，注意这个名字中不能有空格
    // 第二个参数需要传入options句柄
    my_c_publisher_node::my_c_publisher_node(const rclcpp::NodeOptions &options)
        : Node("vel_publisher", options)
    {
        // 创建一个发布者，发布者发布的话题名称为"/turtle1/cmd_vel"
        vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        // 创建一个定时器，周期500ms，需要包含头文件<chrono>命名空间std::chrono_literals
        timer_ = this->create_wall_timer(
            500ms, std::bind(&my_c_publisher_node::vel_cb, this)); // 创建一个定时器，每隔500ms发布速度指令
    }

    void my_c_publisher_node::vel_cb()
    {
        // 定义一个消息结构，用于存放期望速度
        auto data = geometry_msgs::msg::Twist();
        vel_gain += 0.1;
        // 为期望速度赋值
        data.angular.z = 1;
        data.linear.x = 2 + vel_gain;
        data.linear.y = 1;
        // 在终端发布消息
        RCLCPP_INFO(this->get_logger(), "Velocity command published!");
        vel_pub_->publish(data); // 将data里面的数据发布到"/turtle1/cmd_vel"话题下面
    }
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(my_composition::my_c_publisher_node)