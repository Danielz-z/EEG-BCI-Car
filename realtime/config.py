from dataclasses import dataclass, field
from pathlib import Path


@dataclass
class RealtimeConfig:
    model_path: str = "training/outputs/models/lstm_4class.keras"
    csv_path: str = "data/naobo.csv"
    serial_port: str = "COM3"
    baudrate: int = 9600
    timer_interval_ms: int = 1000

    sampling_rate: int = 128
    lowcut: float = 0.5
    highcut: float = 45.0
    filter_order: int = 5

    num_rows_to_read: int = 256
    num_last_rows: int = 52
    time_steps: int = 2

    confidence_threshold: float = 0.5
    threshold: float = 0.8
    right_lane_change_threshold: float = 0.4
    slow_down_threshold: float = 0.4
    left_lane_change_threshold: float = 0.4

    initial_lane: int = 3
    max_lane: int = 5
    min_lane: int = 1

    enable_send_commands: bool = False

    reference_columns: list[str] = field(default_factory=lambda: [
        "RawData", "Attention", "Delta", "HighAlpha", "HighBeta",
        "new_column_25", "new_column_26", "new_column_27", "new_column_28",
        "new_column_29", "new_column_30", "new_column_31", "new_column_32",
        "mean", "std", "max", "min", "rms", "dominant_freq", "psd_mean", "psd_max"
    ])

    label_names: dict[int, str] = field(default_factory=lambda: {
        0: "Decelerate",
        1: "LaneChangeLeft",
        2: "LaneChangeRight",
        3: "InvalidCommand",
    })

    command_mapping: dict[str, str] = field(default_factory=lambda: {
        "Decelerate": "slow",
        "LaneChangeLeft": "left",
        "LaneChangeRight": "righ",
        "Advance": "adva",
        "Stop": "stop",
    })


def ensure_runtime_paths(cfg: RealtimeConfig) -> None:
    Path(cfg.model_path).parent.mkdir(parents=True, exist_ok=True)
    Path(cfg.csv_path).parent.mkdir(parents=True, exist_ok=True)