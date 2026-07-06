from .datatypes import MotorCommand
from .constants import START_CHAR, END_CHAR


class PacketBuilder:

    @staticmethod
    def build_command(command: MotorCommand) -> str:
        return (
            f"{START_CHAR}"
            f"{command.linear_velocity:.3f},"
            f"{command.angular_velocity:.3f}"
            f"{END_CHAR}\n"
        )