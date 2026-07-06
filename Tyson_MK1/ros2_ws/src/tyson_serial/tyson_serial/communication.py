import serial


class Communication:

    def __init__(self, port: str, baudrate: int, timeout: float):

        self.serial = serial.Serial(
            port=port,
            baudrate=baudrate,
            timeout=timeout
        )

        self.serial.reset_input_buffer()
        self.serial.reset_output_buffer()

    def read_line(self) -> str:

        return self.serial.readline().decode(errors="ignore").strip()

    def write_line(self, packet: str):

        self.serial.write(packet.encode())

    def connected(self) -> bool:

        return self.serial.is_open