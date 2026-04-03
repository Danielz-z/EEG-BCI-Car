from __future__ import annotations

import pandas as pd
import numpy as np

from inference.config import InferenceConfig
from realtime.data.reader import read_csv_data, get_latest_window
from realtime.signal.features import build_feature_row, create_sequences
from realtime.model.predictor import Predictor


class OfflineInference:
    def __init__(self, cfg: InferenceConfig):
        self.cfg = cfg
        self.predictor = Predictor(cfg.model_path)
        self.feature_buffer = []

    def run_once(self):
        df = read_csv_data(self.cfg.csv_path)
        window_df = get_latest_window(df, self.cfg.num_rows_to_read)
        if window_df is None:
            raise ValueError("Not enough rows in CSV for inference.")

        feature_row = build_feature_row(window_df, self.cfg.sampling_rate)
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
        pred_idx, confidence, probs = self.predictor.predict(X_seq[-1:])

        return {
            "pred_idx": pred_idx,
            "pred_label": self.cfg.label_names[pred_idx],
            "confidence": confidence,
            "probabilities": probs.tolist(),
        }