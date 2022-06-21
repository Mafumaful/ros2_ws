import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist

'''创建一个节点类'''


class my_py_publisher_node(Node):

    def __init__(self):
        # 初始化节点，节点名称是vel_publisher注意这里的节点名字中间不能带有空格
        super().__init__('vel_publisher')
        # 指定发布的话题名称，为'/turtle1/cmd_vel'
        self.publisher_ = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)
        # 创建参数，初始化值为1
        self.declare_parameter('speed', 1.0)
        # 定时器的间隔为0.5秒
        timer_period = 0.5  # seconds
        timer_period_2 = 1
        # 创建一个定时器，周期性进入回调函数，回调函数名称为timer_callback
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.timer_2 = self.create_timer(timer_period_2, self.timer2_callback)
        self.vel_ = 0

    def timer_callback(self):
        # 定义一个Twist结构，用于存放期望速度
        data = Twist()
        # 输入期望速度
        data.linear.x = float(self.vel_)
        data.angular.z = 1.0
        # 在终端发布消息
        # self.get_logger().info('Velocity command published!')
        # 发布消息
        self.publisher_.publish(data)

    def timer2_callback(self):
        self.vel_ = self.get_parameter(
            'speed').get_parameter_value().double_value
        self.get_logger().info("the velocity is %f" % self.vel_)


def main(args=None):
    rclpy.init(args=args)
    # 创建发布者的实例
    my_py_publisher = my_py_publisher_node()
    # 进入自旋锁
    rclpy.spin(my_py_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    my_py_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
