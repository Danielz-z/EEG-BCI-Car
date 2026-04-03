from __future__ import annotations

import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler


def read_excel_data(file_path: str, label_col: str, sheet_name: str = "Sheet1"):
    df = pd.read_excel(file_path, sheet_name=sheet_name)

    if label_col not in df.columns:
        raise ValueError(f"Column '{label_col}' not found in dataset.")

    X = df.drop(columns=[label_col]).values
    y = df[label_col].values
    return X, y, df


def split_data(X, y, test_size: float = 0.2, random_state: int = 345):
    return train_test_split(
        X,
        y,
        test_size=test_size,
        random_state=random_state,
        stratify=y
    )


def normalize_data(X_train, X_test):
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    return X_train_scaled, X_test_scaled, scaler


def encode_labels(y, num_classes: int):
    y = np.asarray(y).astype(int)
    y_zero_based = y - 1

    if np.min(y_zero_based) < 0 or np.max(y_zero_based) >= num_classes:
        raise ValueError(
            f"Label values out of range. Got labels {np.unique(y)}, "
            f"but num_classes={num_classes}."
        )

    one_hot = np.eye(num_classes, dtype=np.float32)[y_zero_based]
    return one_hot, y_zero_based


def create_sequences(X, y_onehot, time_steps: int):
    X_seq = []
    y_seq = []

    for i in range(len(X) - time_steps + 1):
        X_seq.append(X[i:i + time_steps])
        y_seq.append(y_onehot[i + time_steps - 1])

    return np.array(X_seq), np.array(y_seq)


def add_gaussian_noise(X, std: float = 0.1):
    noise = np.random.normal(0, std, X.shape)
    return X + noise