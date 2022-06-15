from turtlesim.srv import Spawn
import rclpy
from rclpy.node import Node

# 继承Node类


class Turtle_client(Node):

    def __init__(self):
        # 给节点命名为turtle_spawn_client
        super().__init__('turtle_spawn_client')
        # 创建一个客户端订阅/spawn下的srv
        self.cli = self.create_client(Spawn, '/spawn')
        # 每一秒进行一次判断，等待服务端
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')
        # 创建一个请求，用来存放发出去的指令
        self.req = Spawn.Request()

    def send_request(self):
        # 给指令赋值，name是海龟的名字，x是小海龟生成的x轴，y是小海龟生成的y轴
        self.req.name = "another_turtle"
        self.req.x = 2.0
        self.req.y = 2.0
        self.req.theta = 0.0
        self.future = self.cli.call_async(self.req)


def main(args=None):
    # 初始化节点
    rclpy.init(args=args)

    # 创建一个客户类
    client = Turtle_client()
    # 发送请求
    client.send_request()

    # 循环
    while rclpy.ok():
        rclpy.spin_once(client)
        # 如果服务端在运行客户端节点以后生成
        if client.future.done():
            try:
                response = client.future.result()
            # 如果发生了错误
            except Exception as e:
                client.get_logger().info(
                    'Service call failed')
            else:
                # 打印回复结果
                client.get_logger().info(
                    'the new turtle name is %s' %
                    response.name)
            break

    # 消除
    client.destroy_node()
    # 关闭节点
    rclpy.shutdown()


if __name__ == '__main__':
    main()
