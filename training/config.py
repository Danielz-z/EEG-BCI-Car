from dataclasses import dataclass
from pathlib import Path


@dataclass
class TrainConfig:
    data_path: str
    label_col: str = "Distraction"
    sheet_name: str = "Sheet1"

    test_size: float = 0.2
    random_state: int = 345

    model_name: str = "bp"   # bp / lstm / gru / svm / logistic
    num_classes: int = 4
    input_dim: int = 21
    time_steps: int = 2

    epochs: int = 50
    batch_size: int = 32
    learning_rate: float = 1e-3

    add_noise: bool = False
    noise_std: float = 0.1

    save_dir: str = "training/outputs/models"
    figure_dir: str = "training/outputs/figures"


def ensure_dirs(cfg: TrainConfig) -> None:
    Path(cfg.save_dir).mkdir(parents=True, exist_ok=True)
    Path(cfg.figure_dir).mkdir(parents=True, exist_ok=True)