import rclpy
from rclpy.node import Node
from std_srvs.srv import Trigger
from geometry_msgs.msg import Twist


class my_py_srv_node(Node):

    def __init__(self):
        super().__init__('my_server')
        self.publisher_ = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)

        timer_period = 0.5
        self.timer = self.create_timer(timer_period, self.timer_callback)

        self.srv = self.create_service(Trigger, 'speed_up', self.srv_cb)

        self.data = Twist()
        self.speed = 0

        self.get_logger().info('The server is ready!')

    def timer_callback(self):
        self.publisher_.publish(self.data)

    def srv_cb(self, request, response):
        self.data.angular.z = 1.0
        self.speed += 0.5
        self.data.linear.x = self.speed
        self.get_logger().info(
            'The speed of the turtle was updated!\r\nThe speed of x is %f' % self.speed)

        return response


def main(args=None):
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
