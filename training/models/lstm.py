from tensorflow import keras


def build_lstm(input_shape, num_classes: int):
    model = keras.models.Sequential([
        keras.layers.Input(shape=input_shape),
        keras.layers.LSTM(128, return_sequences=True),
        keras.layers.LSTM(32),
        keras.layers.Dense(num_classes, activation="softmax"),
    ])
    return model