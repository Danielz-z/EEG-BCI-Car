from __future__ import annotations

import sys
import pandas as pd
import numpy as np
from PyQt5.QtWidgets import QApplication

from realtime.config import RealtimeConfig, ensure_runtime_paths
from realtime.data.reader import read_csv_data, get_latest_window
from realtime.signal.features import build_feature_row, create_sequences
from realtime.model.predictor import Predictor
from realtime.hardware.bluetooth import BluetoothController
from realtime.control.controller import CommandController
from realtime.ui.app import MainWindow


class RealtimePipeline:
    def __init__(self, cfg: RealtimeConfig):
        self.cfg = cfg
        self.predictor = Predictor(cfg.model_path)
        self.bt = BluetoothController(cfg.serial_port, cfg.baudrate)
        self.controller = CommandController(cfg, self.bt)
        self.feature_buffer = []

    def step(self):
        try:
            df = read_csv_data(self.cfg.csv_path)
            window_df = get_latest_window(df, self.cfg.num_rows_to_read)
            if window_df is None:
                return None

            feature_row = build_feature_row(window_df, sampling_rate=self.cfg.sampling_rate)

            for col in self.cfg.reference_columns:
                if col not in feature_row:
                    feature_row[col] = 0.0

            ordered_features = [feature_row[col] for col in self.cfg.reference_columns]
            self.feature_buffer.append(ordered_features)

            if len(self.feature_buffer) < self.cfg.time_steps:
                return None

            feature_df = pd.DataFrame(self.feature_buffer, columns=self.cfg.reference_columns)
            X = feature_df.values.astype(np.float32)
            X_seq = create_sequences(X, self.cfg.time_steps)

            pred_idx, confidence, _ = self.predictor.predict(X_seq[-1:])
            pred_label = self.cfg.label_names.get(pred_idx, "InvalidCommand")

            command = self.controller.decide_command(pred_label, confidence)
            self.controller.execute_command(command)

            eeg_band_values = [
                feature_row.get("Delta", 0.0),
                0.0,
                0.0,
                feature_row.get("HighAlpha", 0.0),
                0.0,
                feature_row.get("HighBeta", 0.0),
                0.0,
                0.0,
            ]

            return {
                "pred_label": pred_label,
                "confidence": confidence,
                "signal_value": feature_row.get("RawData", 0.0),
                "attention_value": feature_row.get("Attention", 0.0),
                "lane": self.controller.current_lane,
                "eeg_band_values": eeg_band_values,
            }

        except FileNotFoundError:
            return None
        except Exception as e:
            print(f"[RealtimePipeline] error: {e}")
            return None


def main():
    cfg = RealtimeConfig()
    ensure_runtime_paths(cfg)

    app = QApplication(sys.argv)
    pipeline = RealtimePipeline(cfg)
    window = MainWindow(cfg, pipeline)
    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()