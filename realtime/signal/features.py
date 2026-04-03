from __future__ import annotations

import numpy as np
import pandas as pd
from scipy.fftpack import fft
from scipy.signal import welch


def extract_time_features(signal) -> dict:
    signal = np.asarray(signal, dtype=float)
    if signal.size == 0:
        return {"mean": 0.0, "std": 0.0, "max": 0.0, "min": 0.0, "rms": 0.0}

    return {
        "mean": float(np.mean(signal)),
        "std": float(np.std(signal)),
        "max": float(np.max(signal)),
        "min": float(np.min(signal)),
        "rms": float(np.sqrt(np.mean(signal ** 2))),
    }


def extract_frequency_features(signal, sampling_rate: int = 128) -> dict:
    signal = np.asarray(signal, dtype=float)
    if signal.size < 2:
        return {"dominant_freq": 0.0, "psd_mean": 0.0, "psd_max": 0.0}

    fft_values = fft(signal)
    fft_magnitude = np.abs(fft_values[: len(fft_values) // 2])
    freqs = np.fft.fftfreq(len(signal), d=1 / sampling_rate)[: len(fft_values) // 2]

    dominant_freq = float(freqs[np.argmax(fft_magnitude)]) if len(fft_magnitude) > 0 and not np.all(fft_magnitude == 0) else 0.0

    nperseg = len(signal) if len(signal) < 256 else 256
    _, psd = welch(signal, fs=sampling_rate, nperseg=nperseg)
    psd = np.abs(psd)

    return {
        "dominant_freq": dominant_freq,
        "psd_mean": float(np.mean(psd)) if len(psd) > 0 else 0.0,
        "psd_max": float(np.max(psd)) if len(psd) > 0 else 0.0,
    }


def create_sequences(data, time_steps: int = 2):
    X = []
    for i in range(len(data) - time_steps + 1):
        X.append(data[i: i + time_steps])
    return np.array(X)


def build_feature_row(window_df: pd.DataFrame, sampling_rate: int) -> dict:
    raw_signal = window_df["RawData"].astype(float).values

    time_feats = extract_time_features(raw_signal)
    freq_feats = extract_frequency_features(raw_signal, sampling_rate=sampling_rate)

    feature_row = {
        "RawData": float(window_df["RawData"].iloc[-1]) if "RawData" in window_df else 0.0,
        "Attention": float(window_df["Attention"].iloc[-1]) if "Attention" in window_df else 0.0,
        "Delta": float(window_df["Delta"].iloc[-1]) if "Delta" in window_df else 0.0,
        "HighAlpha": float(window_df["HighAlpha"].iloc[-1]) if "HighAlpha" in window_df else 0.0,
        "HighBeta": float(window_df["HighBeta"].iloc[-1]) if "HighBeta" in window_df else 0.0,
        "new_column_25": float(window_df["new_column_25"].iloc[-1]) if "new_column_25" in window_df else 0.0,
        "new_column_26": float(window_df["new_column_26"].iloc[-1]) if "new_column_26" in window_df else 0.0,
        "new_column_27": float(window_df["new_column_27"].iloc[-1]) if "new_column_27" in window_df else 0.0,
        "new_column_28": float(window_df["new_column_28"].iloc[-1]) if "new_column_28" in window_df else 0.0,
        "new_column_29": float(window_df["new_column_29"].iloc[-1]) if "new_column_29" in window_df else 0.0,
        "new_column_30": float(window_df["new_column_30"].iloc[-1]) if "new_column_30" in window_df else 0.0,
        "new_column_31": float(window_df["new_column_31"].iloc[-1]) if "new_column_31" in window_df else 0.0,
        "new_column_32": float(window_df["new_column_32"].iloc[-1]) if "new_column_32" in window_df else 0.0,
        **time_feats,
        **freq_feats,
    }
    return feature_row