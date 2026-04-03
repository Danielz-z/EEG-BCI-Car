from __future__ import annotations

import argparse
from pathlib import Path
import random

import numpy as np
import tensorflow as tf
from tensorflow import keras

from config import TrainConfig, ensure_dirs
from data_utils import (
    add_gaussian_noise,
    create_sequences,
    encode_labels,
    normalize_data,
    read_excel_data,
    split_data,
)
from evaluate import (
    evaluate_classifier,
    print_results,
    save_confusion_matrix,
    save_training_curves,
)
from models import build_bp, build_gru, build_lstm, build_logistic, build_svm


def set_seed(seed: int = 42):
    random.seed(seed)
    np.random.seed(seed)
    tf.random.set_seed(seed)


def parse_args():
    parser = argparse.ArgumentParser(description="Unified EEG model training entry.")
    parser.add_argument("--data_path", type=str, required=True, help="Path to Excel dataset.")
    parser.add_argument("--model", type=str, required=True,
                        choices=["bp", "lstm", "gru", "svm", "logistic"],
                        help="Model type.")
    parser.add_argument("--num_classes", type=int, required=True, help="Number of classes.")
    parser.add_argument("--input_dim", type=int, default=21, help="Input feature dimension.")
    parser.add_argument("--time_steps", type=int, default=2, help="Time steps for LSTM/GRU.")
    parser.add_argument("--epochs", type=int, default=50, help="Training epochs.")
    parser.add_argument("--batch_size", type=int, default=32, help="Batch size.")
    parser.add_argument("--label_col", type=str, default="Distraction", help="Label column name.")
    parser.add_argument("--sheet_name", type=str, default="Sheet1", help="Excel sheet name.")
    parser.add_argument("--add_noise", action="store_true", help="Whether to add Gaussian noise.")
    parser.add_argument("--noise_std", type=float, default=0.1, help="Noise std.")
    return parser.parse_args()


def train_keras_model(model, X_train, y_train, X_test, y_test, epochs, batch_size, save_path):
    model.compile(
        optimizer=keras.optimizers.Adam(),
        loss="categorical_crossentropy",
        metrics=["accuracy"],
    )

    history = model.fit(
        X_train,
        y_train,
        validation_data=(X_test, y_test),
        epochs=epochs,
        batch_size=batch_size,
        verbose=1,
    )

    model.save(save_path)

    y_prob = model.predict(X_test, verbose=0)
    y_pred = np.argmax(y_prob, axis=1)
    y_true = np.argmax(y_test, axis=1)

    return history, y_true, y_pred


def main():
    set_seed(42)
    args = parse_args()

    cfg = TrainConfig(
        data_path=args.data_path,
        label_col=args.label_col,
        sheet_name=args.sheet_name,
        model_name=args.model,
        num_classes=args.num_classes,
        input_dim=args.input_dim,
        time_steps=args.time_steps,
        epochs=args.epochs,
        batch_size=args.batch_size,
        add_noise=args.add_noise,
        noise_std=args.noise_std,
    )

    ensure_dirs(cfg)

    X, y, _ = read_excel_data(cfg.data_path, cfg.label_col, cfg.sheet_name)

    if X.shape[1] != cfg.input_dim:
        print(
            f"[Warning] input_dim={cfg.input_dim}, but dataset feature dim={X.shape[1]}. "
            f"Will use dataset feature dimension automatically."
        )
        cfg.input_dim = X.shape[1]

    X_train, X_test, y_train_raw, y_test_raw = split_data(
        X, y, test_size=cfg.test_size, random_state=cfg.random_state
    )

    X_train, X_test, _ = normalize_data(X_train, X_test)

    model_stem = f"{cfg.model_name}_{cfg.num_classes}class"
    model_save_path = str(Path(cfg.save_dir) / f"{model_stem}.keras")
    curve_save_path = str(Path(cfg.figure_dir) / f"{model_stem}_curves.png")
    cm_save_path = str(Path(cfg.figure_dir) / f"{model_stem}_cm.png")

    if cfg.model_name in ["svm", "logistic"]:
        y_train_idx = np.asarray(y_train_raw).astype(int) - 1
        y_test_idx = np.asarray(y_test_raw).astype(int) - 1

        model = build_svm() if cfg.model_name == "svm" else build_logistic()
        model.fit(X_train, y_train_idx)
        y_pred_idx = model.predict(X_test)

        results = evaluate_classifier(y_test_idx, y_pred_idx)
        print_results(results)
        save_confusion_matrix(results["confusion_matrix"], cm_save_path)
        print(f"Saved confusion matrix to: {cm_save_path}")
        return

    y_train_onehot, _ = encode_labels(y_train_raw, cfg.num_classes)
    y_test_onehot, _ = encode_labels(y_test_raw, cfg.num_classes)

    if cfg.add_noise:
        X_train = add_gaussian_noise(X_train, cfg.noise_std)

    if cfg.model_name == "bp":
        model = build_bp(cfg.input_dim, cfg.num_classes)
        history, y_true, y_pred = train_keras_model(
            model=model,
            X_train=X_train,
            y_train=y_train_onehot,
            X_test=X_test,
            y_test=y_test_onehot,
            epochs=cfg.epochs,
            batch_size=cfg.batch_size,
            save_path=model_save_path,
        )

    elif cfg.model_name in ["lstm", "gru"]:
        X_train_seq, y_train_seq = create_sequences(X_train, y_train_onehot, cfg.time_steps)
        X_test_seq, y_test_seq = create_sequences(X_test, y_test_onehot, cfg.time_steps)
        input_shape = (cfg.time_steps, cfg.input_dim)

        model = build_lstm(input_shape, cfg.num_classes) if cfg.model_name == "lstm" else build_gru(input_shape, cfg.num_classes)

        history, y_true, y_pred = train_keras_model(
            model=model,
            X_train=X_train_seq,
            y_train=y_train_seq,
            X_test=X_test_seq,
            y_test=y_test_seq,
            epochs=cfg.epochs,
            batch_size=cfg.batch_size,
            save_path=model_save_path,
        )
    else:
        raise ValueError(f"Unsupported model name: {cfg.model_name}")

    results = evaluate_classifier(y_true, y_pred)
    print_results(results)

    save_training_curves(history, curve_save_path)
    save_confusion_matrix(results["confusion_matrix"], cm_save_path)

    print(f"Saved model to: {model_save_path}")
    print(f"Saved training curves to: {curve_save_path}")
    print(f"Saved confusion matrix to: {cm_save_path}")


if __name__ == "__main__":
    main()