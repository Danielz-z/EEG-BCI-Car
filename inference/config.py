from dataclasses import dataclass


@dataclass
class InferenceConfig:
    model_path: str = "training/outputs/models/lstm_4class.keras"
    csv_path: str = "data/naobo.csv"
    sampling_rate: int = 128
    num_rows_to_read: int = 256
    time_steps: int = 2

    reference_columns: list[str] = None
    label_names: dict[int, str] = None

    def __post_init__(self):
        if self.reference_columns is None:
            self.reference_columns = [
                "RawData", "Attention", "Delta", "HighAlpha", "HighBeta",
                "new_column_25", "new_column_26", "new_column_27", "new_column_28",
                "new_column_29", "new_column_30", "new_column_31", "new_column_32",
                "mean", "std", "max", "min", "rms", "dominant_freq", "psd_mean", "psd_max"
            ]
        if self.label_names is None:
            self.label_names = {
                0: "Decelerate",
                1: "LaneChangeLeft",
                2: "LaneChangeRight",
                3: "InvalidCommand",
            }