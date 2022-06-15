#include "rclcpp/rclcpp.hpp"
#include "service_interfaces/srv/calculate.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

//创建一个类节点，名字叫做Organization,继承自Node.
class Organization : public rclcpp::Node 
{

public:
    // 初始化汉堡总数NumOfAll为100
    Organization() : Node("Organization"), NumOfAll(100)
    {
        // 热心组织的自我介绍
        RCLCPP_INFO(this->get_logger(), "大家好, 我们是热心组织, 我们只给poorer发汉堡.");
        // 实例化回调组, 作用为避免死锁(请自行百度ROS2死锁)
        callback_group_organization = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        // 实例化发汉堡的的服务
        Organization_Server = this->create_service<service_interfaces::srv::Calculate>("Calculate",
                                    std::bind(&Organization::organization_callback,this,_1,_2),
                                    rmw_qos_profile_services_default,
                                    callback_group_organization);
    }
private:
    // 剩余汉堡总数
    size_t NumOfAll;

    // 声明一个服务回调组
    rclcpp::CallbackGroup::SharedPtr callback_group_organization;

    // 声明一个服务端
    rclcpp::Service<service_interfaces::srv::Calculate>::SharedPtr Organization_Server;

    // 声明一个回调函数，当收到要汉堡请求时调用该函数
    void organization_callback(const service_interfaces::srv::Calculate::Request::SharedPtr request,
                               const service_interfaces::srv::Calculate::Response::SharedPtr response)
    {
        // 首先判断是不是穷人
        if(request->status == "Poorer")
        {
            // 打印Poorer家的人数
            RCLCPP_INFO(this->get_logger(), "收到一个来自%s的请求，他家有%d个人.", request->status.c_str(), request->num_of_people);
            // 计算应给汉堡数量，每人两个
            unsigned int NumOfRequired = request->num_of_people*2;
            // 检查剩余汉堡够不够分
            if(NumOfRequired > NumOfAll)
            {
                // 不够分了, 返回false
                RCLCPP_INFO(this->get_logger(), "当前汉堡库里只剩%d个汉堡啦! 已经不够分了, 请明日再来.", NumOfRequired);
                response->success = false;
            }
            else
            {
                // 够分, 更新剩余汉堡数量
                NumOfAll -= NumOfRequired;
                // 返回给几个汉堡
                response->num_of_hamburger = NumOfRequired;
                // 成功送出, 返回true
                response->success = true;
                RCLCPP_INFO(this->get_logger(), "成功送出%d个汉堡~ 还剩余%d个汉堡", NumOfRequired, NumOfAll);
            }
        }
        else
        {
            // 富人不给汉堡, 同样返回false, 同时返回送出汉堡数量为0
            response->success = false;
            response->num_of_hamburger = 0;
            RCLCPP_INFO(this->get_logger(), "收到一个非法请求，这人是个%s, 不满足送汉堡资格.", request->status.c_str());
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Organization>();
    // 把节点的执行器变成多线程执行器, 避免死锁
    rclcpp::executors::MultiThreadedExecutor exector;
    exector.add_node(node);
    exector.spin();
    rclcpp::shutdown();
    return 0;
}
