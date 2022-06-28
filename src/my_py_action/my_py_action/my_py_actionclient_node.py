# 包含头文件
import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node

from my_action.action import Count


class CountActionClient(Node):  # 定义一个Client类，继承Node

    def __init__(self):
        super().__init__('Count_action_client')  # 初始化节点，名称为Count_action_client
        self._action_client = ActionClient(self, Count, 'count')  # 服务名绑定为count
        self.send_goal_timer = self.create_timer(
            0.5, self.send_goal)  # 创建一个定时器，绑定回调函数send_goal

    def send_goal(self):
        # 发送目标
        self.send_goal_timer.cancel()  # 计时器取消循环
        goal_msg = Count.Goal()  # goal_msg类型为Count.Goal()
        goal_msg.goal_num = 10  # 设置目标数字为10
        self._action_client.wait_for_server()  # 等待服务器
        self.get_logger().info('Sending goal')
        # 绑定反馈回调函数feedback_cb
        self._send_goal_future = self._action_client.send_goal_async(
            goal_msg, feedback_callback=self.feedback_cb)
        # 绑定应答回调函数goal_response_callback
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self, future):
        """收到应答结果"""
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info('Goal was rejected by server')
            return
        self.get_logger().info('Goal accepted by server, waiting for result')
        self._get_result_future = goal_handle.get_result_async()
        # 绑定结果回调函数
        self._get_result_future.add_done_callback(self.get_result_callback)

    def get_result_callback(self, future):
        """获取结果反馈"""
        result = future.result().result
        # 打印结果内容
        self.get_logger().info(f'Result received: {result.finish}')
        rclpy.shutdown()

    def feedback_cb(self, future):
        # 打印反馈内容
        feedback = future.feedback
        self.get_logger().info(
            f'The progress is: {feedback.percent_complete}%')


def main(args=None):
    rclpy.init(args=args)
    action_client = CountActionClient()
    rclpy.spin(action_client)


if __name__ == '__main__':
    main()
