#!/usr/bin/env python3

import math

import rclpy
from rclpy.node import Node

from nav_msgs.msg import Odometry
from geometry_msgs.msg import TransformStamped

from tf2_ros import TransformBroadcaster

from tyson_interfaces.msg import WheelData

from .parameters import *


class OdometryNode(Node):

    def __init__(self):

        super().__init__("odometry_node")

        self.create_subscription(WheelData,"/wheel_data",self.wheel_callback,10)

        self.odom_pub = self.create_publisher(Odometry,"/odom",10)

        self.tf_broadcaster = TransformBroadcaster(self)

        self.x = 0.0
        self.y = 0.0
        self.theta = 0.0

        self.linear_velocity = 0.0
        self.angular_velocity = 0.0

        self.prev_fl = None
        self.prev_fr = None
        self.prev_rl = None
        self.prev_rr = None

        self.last_time = None

        self.get_logger().info("Odometry Node Started")


    def wheel_callback(self, msg):

        current_time = msg.stamp.sec + msg.stamp.nanosec * 1e-9

        if self.last_time is None:

            self.last_time = current_time

            self.prev_fl = msg.tick_fl
            self.prev_fr = msg.tick_fr
            self.prev_rl = msg.tick_rl
            self.prev_rr = msg.tick_rr

            return

        dt = current_time - self.last_time

        if dt <= 0.0:
            return

        # -------------------------------------

        delta_fl = msg.tick_fl - self.prev_fl
        delta_fr = msg.tick_fr - self.prev_fr
        delta_rl = msg.tick_rl - self.prev_rl
        delta_rr = msg.tick_rr - self.prev_rr

        self.prev_fl = msg.tick_fl
        self.prev_fr = msg.tick_fr
        self.prev_rl = msg.tick_rl
        self.prev_rr = msg.tick_rr

        self.last_time = current_time

        # -------------------------------------

        fl = self.compute_distance(delta_fl)
        fr = self.compute_distance(delta_fr)
        rl = self.compute_distance(delta_rl)
        rr = self.compute_distance(delta_rr)

        left_distance = (fl + rl) / 2.0
        right_distance = (fr + rr) / 2.0

        distance = (left_distance + right_distance) / 2.0

        delta_theta = (right_distance - left_distance) / TRACK_WIDTH

        self.linear_velocity = distance / dt
        self.angular_velocity = delta_theta / dt

        self.theta += delta_theta

        self.x += distance * math.cos(self.theta)
        self.y += distance * math.sin(self.theta)

        self.publish_odometry(msg)
        self.publish_tf(msg)


    # =========================================================

    def compute_distance(self, delta_ticks):

        return ((2.0 * math.pi * WHEEL_RADIUS) * (delta_ticks / ENCODER_CPR))


    # =========================================================

    def publish_odometry(self, msg):

        odom = Odometry()

        odom.header.stamp = msg.stamp
        odom.header.frame_id = ODOM_FRAME
        odom.child_frame_id = BASE_FRAME

        odom.pose.pose.position.x = self.x
        odom.pose.pose.position.y = self.y
        odom.pose.pose.position.z = 0.0

        odom.pose.pose.orientation.z = math.sin(self.theta / 2.0)
        odom.pose.pose.orientation.w = math.cos(self.theta / 2.0)

        odom.twist.twist.linear.x = self.linear_velocity
        odom.twist.twist.angular.z = self.angular_velocity

        self.odom_pub.publish(odom)


    # =========================================================

    def publish_tf(self, msg):

        tf = TransformStamped()

        tf.header.stamp = msg.stamp

        tf.header.frame_id = ODOM_FRAME
        tf.child_frame_id = BASE_FRAME

        tf.transform.translation.x = self.x
        tf.transform.translation.y = self.y
        tf.transform.translation.z = 0.0

        tf.transform.rotation.z = math.sin(self.theta / 2.0)
        tf.transform.rotation.w = math.cos(self.theta / 2.0)

        self.tf_broadcaster.sendTransform(tf)


# =============================================================


def main(args=None):

    rclpy.init(args=args)

    node = OdometryNode()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == "__main__":
    main()