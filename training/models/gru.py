from tensorflow import keras


def build_gru(input_shape, num_classes: int):
    model = keras.models.Sequential([
        keras.layers.Input(shape=input_shape),
        keras.layers.GRU(128, return_sequences=True),
        keras.layers.GRU(32),
        keras.layers.Dense(num_classes, activation="softmax"),
    ])
    return model