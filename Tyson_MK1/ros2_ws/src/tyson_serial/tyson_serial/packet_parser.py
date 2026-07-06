from .datatypes import EncoderData, ImuData
from .constants import START_CHAR, END_CHAR


class PacketParser:

    EXPECTED_FIELDS = 15

    @staticmethod
    def parse(packet: str):

        packet = packet.strip()

        if not packet.startswith(START_CHAR):
            return None

        if not packet.endswith(END_CHAR):
            return None

        packet = packet[1:-1]

        fields = packet.split(",")

        if len(fields) != PacketParser.EXPECTED_FIELDS:
            return None

        try:

            encoder = EncoderData()

            encoder.timestamp = int(fields[0])

            encoder.ticks[0] = int(fields[1])
            encoder.ticks[1] = int(fields[2])
            encoder.ticks[2] = int(fields[3])
            encoder.ticks[3] = int(fields[4])

            imu = ImuData()

            imu.timestamp = encoder.timestamp

            imu.ax = float(fields[5])
            imu.ay = float(fields[6])
            imu.az = float(fields[7])

            imu.gx = float(fields[8])
            imu.gy = float(fields[9])
            imu.gz = float(fields[10])

            imu.qx = float(fields[11])
            imu.qy = float(fields[12])
            imu.qz = float(fields[13])
            imu.qw = float(fields[14])

            return encoder, imu

        except ValueError:

            return None