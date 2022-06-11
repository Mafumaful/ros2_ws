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
        # 定时器的间隔为0.5秒
        timer_period = 0.5  # seconds
        # 创建一个定时器，周期性进入回调函数，回调函数名称为timer_callback
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.vel_gain = 0

    def timer_callback(self):
        # 定义一个Twist结构，用于存放期望速度
        data = Twist()
        # 输入期望速度
        self.vel_gain += 0.1
        data.angular.z = 1.
        data.linear.x = 2+self.vel_gain
        data.linear.y = 1.
        # 在终端发布消息
        self.get_logger().info('Velocity command published!')
        # 发布消息
        self.publisher_.publish(data)


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
