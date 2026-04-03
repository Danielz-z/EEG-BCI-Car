from __future__ import annotations

import numpy as np
from tensorflow import keras


class Predictor:
    def __init__(self, model_path: str):
        self.model = keras.models.load_model(
            model_path,
            custom_objects={"LeakyReLU": keras.layers.LeakyReLU},
        )

    def predict(self, X):
        probs = self.model.predict(X, verbose=0)
        pred_idx = int(np.argmax(probs, axis=1)[0])
        confidence = float(np.max(probs))
        return pred_idx, confidence, probs[0]