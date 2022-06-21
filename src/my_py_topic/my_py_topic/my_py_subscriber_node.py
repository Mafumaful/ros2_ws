# 包含相应的头文件
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
才ear才ear
'''创建一个节点类'''


class VelSubscriber(Node):

    def __init__(self):
        # 节点初始化，规定速度订阅节点名称
        super().__init__('vel_subscriber')
        # 速度订阅者，确定速度订阅话题
        self.subscription = self.create_subscription(
            Twist,
            'turtle1/cmd_vel',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning

    # 回调函数
    def listener_callback(self, msg):
        self.get_logger().info(
            '\r\n the speed of x is: %f\r\n the speed of y is %f\r\n the speed of angular z is %f\r\n' % (msg.linear.x, msg.linear.y, msg.angular.z))


def main(args=None):
    # 初始化节点
    rclpy.init(args=args)
    vel_subscriber = VelSubscriber()
    rclpy.spin(vel_subscriber)
    vel_subscriber.destroy_node()
    rclpy.shutdown()


# 主函数
if __name__ == '__main__':
    main()
