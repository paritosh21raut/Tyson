#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist, TwistStamped


class CmdVelBridge(Node):

    def __init__(self):
        super().__init__("cmd_vel_bridge")

        self.publisher = self.create_publisher(
            TwistStamped,
            "/diff_drive_controller/cmd_vel",
            10,
        )

        self.subscription = self.create_subscription(
            Twist,
            "/cmd_vel",
            self.callback,
            10,
        )

    def callback(self, msg: Twist):
        out = TwistStamped()
        out.header.stamp = self.get_clock().now().to_msg()
        out.header.frame_id = "base_link"
        out.twist = msg

        self.publisher.publish(out)


def main(args=None):
    rclpy.init(args=args)

    node = CmdVelBridge()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()