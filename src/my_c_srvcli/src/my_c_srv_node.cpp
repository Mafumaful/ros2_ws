// 头文件
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>

// 用于定时器
using namespace std::chrono_literals;

// 继承自node
class my_c_srv_node : public rclcpp::Node
{
private:
    double _speed = 0;
    geometry_msgs::msg::Twist _data = geometry_msgs::msg::Twist();
    // 速度指令的发布者
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub_;
    // 服务
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr vel_ser_;
    // 服务的回调函数
    void server_cb(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                   std::shared_ptr<std_srvs::srv::Trigger::Response> response);
    // 定时器的回调函数
    void timer_cb(void);
    // 定时器
    rclcpp::TimerBase::SharedPtr timer_;

public:
    // 构造函数
    my_c_srv_node();
};

// 服务的回调函数
void my_c_srv_node::server_cb(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                              std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
    // 设置乌龟x轴线速度+0.5
    _speed += 0.5;
    _data.angular.z = 1;
    _data.linear.x = _speed;
    // 打印更新后的速度
    RCLCPP_INFO(this->get_logger(), "The speed of the turtle was updated!\r\nThe speed of x is %f", _speed);
}

my_c_srv_node::my_c_srv_node() : Node("my_server")
{
    // 创建一个发布者，发布者发布的话题名称为"/turtle1/cmd_vel",队列长度为10
    vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    // 创建一个服务，服务话题是speed_up，回调函数是server_cb，消息类型是std_srvs::srv::Trigger
    vel_ser_ = this->create_service<std_srvs::srv::Trigger>("speed_up",
                                                            std::bind(&my_c_srv_node::server_cb, this, std::placeholders::_1, std::placeholders::_2));
    // 创建一个定时器，每隔一段时间就调用一次回调函数
    timer_ = this->create_wall_timer(500ms, std::bind(&my_c_srv_node::timer_cb, this));
    // 打印初始化完成信息
    RCLCPP_INFO(this->get_logger(), "The server is ready!");
}

// 定时器的回调函数
void my_c_srv_node::timer_cb(void)
{
    // 每隔一段时间就发布一次速度控制指令
    vel_pub_->publish(_data);
}

int main(int argc, char const **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    // 使用rclcpp::spin创建my_c_srv_node，进入自旋锁
    rclcpp::spin(std::make_shared<my_c_srv_node>());
    rclcpp::shutdown();
    return 0;
}
