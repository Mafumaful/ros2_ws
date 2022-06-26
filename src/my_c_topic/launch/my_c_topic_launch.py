from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    turtlesim_node = Node(
        package='turtlesim',
        node_executable='turtlesim_node',
    )
    my_py_publisher_node = Node(
        package='my_c_topic',
        node_executable='my_c_publisher_node',
        output='screen'
    )
    return LaunchDescription([
        turtlesim_node,
        my_py_publisher_node
    ])
