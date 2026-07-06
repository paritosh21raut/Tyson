from launch import LaunchDescription

from launch_ros.actions import Node


def generate_launch_description():

    return LaunchDescription([

        Node(
            package="tyson_odometry",
            executable="odometry_node",
            name="odometry_node",
            output="screen"
        )

    ])