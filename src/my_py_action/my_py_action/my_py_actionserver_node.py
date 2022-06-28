import time

import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node

from my_action.action import Count


class CountActionServer(Node):

    def __init__(self):
        super().__init__('Count_action_server')
        self._action_server = ActionServer(
            self,
            Count,
            'count',
            self.execute_callback,
            callback_group=None
        )

    def execute_callback(self, goal_handle):
        self.get_logger().info('Received goal, target is %d' %
                               goal_handle.request.goal_num)

        self.get_logger().info('Executing goal')

        feedback_msg = Count.Feedback()

        for i in range(1, goal_handle.request.goal_num+1):
            result = Count.Result()

            feedback_msg.percent_complete = int(
                100/goal_handle.request.goal_num*i)
            goal_handle.publish_feedback(feedback_msg)
            self.get_logger().info('Publish feedback')
            time.sleep(0.1)
            # self.get_logger().info('Feedback: %d%s' % (feedback_msg.percent_complete*i, "%"))

        if rclpy.ok():
            result.finish = goal_handle.request.goal_num
            goal_handle.succeed()
            self.get_logger().info('Goal succeeded')

        return result


def main(args=None):
    rclpy.init(args=args)

    Count_action_server = CountActionServer()

    rclpy.spin(Count_action_server)


if __name__ == '__main__':
    main()
