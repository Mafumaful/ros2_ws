# 包含头文件
import time

import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node

from my_action.action import Count


class CountActionServer(Node):  # 定义一个Client类，继承Node

    # 构造函数
    def __init__(self):
        # 创建一个新的节点，节点名称为Count_action_server
        super().__init__('Count_action_server')
        self._action_server = ActionServer(
            self,
            Count,
            'count',
            self.execute_callback
        )

    def execute_callback(self, goal_handle):  # 执行回调函数，当收到目标消息的时候，会进入这个函数
        self.get_logger().info('Received goal, target is %d' %
                               goal_handle.request.goal_num)

        self.get_logger().info('Executing goal')

        # 反馈消息为Count.Feedback()类型
        feedback_msg = Count.Feedback()

        for i in range(1, goal_handle.request.goal_num+1):
            # 结果为Count.Result()类型
            result = Count.Result()

            # 给反馈赋值
            feedback_msg.percent_complete = int(
                100/goal_handle.request.goal_num*i)
            # 发布反馈处理结果
            goal_handle.publish_feedback(feedback_msg)
            self.get_logger().info('Publish feedback')
            # 延时函数
            time.sleep(0.1)

        if rclpy.ok():
            result.finish = goal_handle.request.goal_num
            goal_handle.succeed()
            self.get_logger().info('Goal succeeded')

        return result


def main(args=None):
    # 初始化
    rclpy.init(args=args)
    Count_action_server = CountActionServer()
    rclpy.spin(Count_action_server)


if __name__ == '__main__':
    main()
