import threading

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist
from sensor_msgs.msg import Imu

from tyson_interfaces.msg import WheelData

from .constants import *
from .communication import Communication
from .packet_builder import PacketBuilder
from .packet_parser import PacketParser
from .datatypes import MotorCommand


class SerialNode(Node):

    def __init__(self):

        super().__init__("serial_node")

        # ---------------- Communication ----------------

        self.communication = Communication(
            PORT,
            BAUD_RATE,
            TIMEOUT
        )

        # ---------------- State ----------------

        self.command = MotorCommand()

        self.lock = threading.Lock()

        self.running = True

        # ---------------- ROS ----------------

        self.cmd_sub = self.create_subscription(
            Twist,
            CMD_VEL_TOPIC,
            self.cmd_callback,
            10
        )

        self.imu_pub = self.create_publisher(
            Imu,
            IMU_TOPIC,
            10
        )

        self.encoder_pub = self.create_publisher(
            WheelData,
            WHEEL_TOPIC,
            10
        )

        # ---------------- Timers ----------------

        self.tx_timer = self.create_timer(
            1.0 / TX_RATE,
            self.tx_callback
        )

        # ---------------- RX Thread ----------------

        self.rx_thread = threading.Thread(
            target=self.rx_loop,
            daemon=True
        )

        self.rx_thread.start()

    # ==================================================

    def cmd_callback(self, msg: Twist):

        with self.lock:

            self.command.linear_velocity = msg.linear.x
            self.command.angular_velocity = msg.angular.z

    # ==================================================

    def tx_callback(self):

        with self.lock:

            packet = PacketBuilder.build_command(self.command)

        self.communication.write_line(packet)

    # ==================================================

    def rx_loop(self):

        while self.running:

            packet = self.communication.read_line()
            self.get_logger().info(f"Read: {repr(packet)}")

            if not packet:
                continue

            result = PacketParser.parse(packet)
            self.get_logger().info(f"RAW: {repr(packet)}")

            if result is None:
                return

            encoder, imu = result

            self.publish_encoder(encoder)
            self.publish_imu(imu)

    # ==================================================

    def publish_encoder(self, encoder):

        msg = WheelData()

        msg.tick_fl = encoder.ticks[0]
        msg.tick_fr = encoder.ticks[1]
        msg.tick_rl = encoder.ticks[2]
        msg.tick_rr = encoder.ticks[3]

        msg.stamp = self.get_clock().now().to_msg()

        self.encoder_pub.publish(msg)

    # ==================================================

    def publish_imu(self, imu):

        msg = Imu()

        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "imu_link"

        msg.linear_acceleration.x = imu.ax
        msg.linear_acceleration.y = imu.ay
        msg.linear_acceleration.z = imu.az

        msg.angular_velocity.x = imu.gx
        msg.angular_velocity.y = imu.gy
        msg.angular_velocity.z = imu.gz

        msg.orientation.x = imu.qx
        msg.orientation.y = imu.qy
        msg.orientation.z = imu.qz
        msg.orientation.w = imu.qw

        self.imu_pub.publish(msg)

    # ==================================================

    def destroy_node(self):

        self.running = False

        self.rx_thread.join(timeout=1)

        super().destroy_node()


def main():

    rclpy.init()

    node = SerialNode()

    rclpy.spin(node)

    node.destroy_node()

    rclpy.shutdown()


if __name__ == "__main__":
    main()