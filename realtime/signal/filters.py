from __future__ import annotations

from scipy.signal import butter, lfilter


def butter_bandpass(lowcut: float, highcut: float, fs: int, order: int = 5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype="band")
    return b, a


def butter_bandpass_filter(data, lowcut: float, highcut: float, fs: int, order: int = 5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    return lfilter(b, a, data)