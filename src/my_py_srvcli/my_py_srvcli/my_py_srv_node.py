# 包含头文件
import rclpy
from rclpy.node import Node
from std_srvs.srv import Trigger
from geometry_msgs.msg import Twist

'''创建一个节点类'''


class my_py_srv_node(Node):

    def __init__(self):
        # 初始化节点，节点名称是my_server
        super().__init__('my_server')
        # 指定发布的话题名称，为'/turtle1/cmd_vel'
        self.publisher_ = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)

        # 定时器的间隔定为0.5秒
        timer_period = 0.5
        # 创建一个定时器，周期性进入回调函数，回调函数名称是timer_callback
        self.timer = self.create_timer(timer_period, self.timer_callback)

        # 创建一个服务，回调函数是server_cb，消息类型是Trigger
        self.srv = self.create_service(Trigger, 'speed_up', self.srv_cb)

        # 创建一个Twist()类，用于存放发布的数据
        self.data = Twist()
        self.speed = 0

        # 打印初始化完成信息
        self.get_logger().info('The server is ready!')

    # 定时器的回调函数
    def timer_callback(self):
        # 每个一段时间就发布一次速度指令
        self.publisher_.publish(self.data)

    # 服务回调函数
    def srv_cb(self, request, response):
        # 设置乌龟x轴线速度+0.5
        self.data.angular.z = 1.0
        self.speed += 0.5
        self.data.linear.x = self.speed
        # 打印更新后的速度
        self.get_logger().info(
            'The speed of the turtle was updated!\r\nThe speed of x is %f' % self.speed)

        return response


def main(args=None):
    # 初始化节点
    rclpy.init(args=args)
    my_py_srv = my_py_srv_node()
    rclpy.spin(my_py_srv)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    my_py_srv.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
