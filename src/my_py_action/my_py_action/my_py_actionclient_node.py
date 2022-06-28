import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node

from my_action.action import Count


class CountActionClient(Node):

    def __init__(self):
        super().__init__('Count_action_client')
        self._action_client = ActionClient(self, Count, 'count')
        self.send_goal_timer = self.create_timer(0.5, self.send_goal)

    def send_goal(self):
        self.send_goal_timer.cancel()
        goal_msg = Count.Goal()
        goal_msg.goal_num = 10
        self._action_client.wait_for_server()
        self.get_logger().info('Sending goal')
        self._send_goal_future = self._action_client.send_goal_async(
            goal_msg, feedback_callback=self.feedback_cb)
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self, future):
        """收到目标处理结果"""
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info('Goal was rejected by server')
            return
        self.get_logger().info('Goal accepted by server, waiting for result')
        self._get_result_future = goal_handle.get_result_async()

    def get_result_callback(self, future):
        """获取结果反馈"""
        result = future.result().result
        self.get_logger().info(f'Result: {result.finish}')

    def feedback_cb(self, future):
        feedback = future.feedback
        self.get_logger().info(
            f'The progress is: {feedback.percent_complete}%')


def main(args=None):
    rclpy.init(args=args)
    action_client = CountActionClient()
    rclpy.spin(action_client)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
