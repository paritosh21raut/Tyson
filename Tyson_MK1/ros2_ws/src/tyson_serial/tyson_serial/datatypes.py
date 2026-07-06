from dataclasses import dataclass, field


@dataclass
class MotorCommand:
    linear_velocity: float = 0.0
    angular_velocity: float = 0.0


@dataclass
class EncoderData:
    ticks: list[int] = field(default_factory=lambda: [0, 0, 0, 0])
    timestamp: int = 0


@dataclass
class ImuData:
    ax: float = 0.0
    ay: float = 0.0
    az: float = 0.0

    gx: float = 0.0
    gy: float = 0.0
    gz: float = 0.0

    qx: float = 0.0
    qy: float = 0.0
    qz: float = 0.0
    qw: float = 1.0

    timestamp: int = 0