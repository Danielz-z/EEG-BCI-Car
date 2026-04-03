from __future__ import annotations

import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix, f1_score


def evaluate_classifier(y_true_idx, y_pred_idx):
    results = {
        "accuracy": accuracy_score(y_true_idx, y_pred_idx),
        "macro_f1": f1_score(y_true_idx, y_pred_idx, average="macro"),
        "confusion_matrix": confusion_matrix(y_true_idx, y_pred_idx),
        "classification_report": classification_report(
            y_true_idx, y_pred_idx, digits=4
        ),
    }
    return results


def print_results(results: dict):
    print(f"Accuracy: {results['accuracy']:.4f}")
    print(f"Macro-F1: {results['macro_f1']:.4f}")
    print("Confusion Matrix:")
    print(results["confusion_matrix"])
    print("Classification Report:")
    print(results["classification_report"])


def save_training_curves(history, save_path: str):
    plt.figure(figsize=(8, 5))

    if "loss" in history.history:
        plt.plot(history.history["loss"], label="train_loss")
    if "val_loss" in history.history:
        plt.plot(history.history["val_loss"], label="val_loss")
    if "accuracy" in history.history:
        plt.plot(history.history["accuracy"], label="train_accuracy")
    if "val_accuracy" in history.history:
        plt.plot(history.history["val_accuracy"], label="val_accuracy")

    plt.xlabel("Epoch")
    plt.ylabel("Value")
    plt.title("Training Curves")
    plt.legend()
    plt.tight_layout()
    plt.savefig(save_path, dpi=300)
    plt.close()


def save_confusion_matrix(cm: np.ndarray, save_path: str):
    plt.figure(figsize=(6, 5))
    plt.imshow(cm, interpolation="nearest")
    plt.title("Confusion Matrix")
    plt.colorbar()
    plt.xlabel("Predicted Label")
    plt.ylabel("True Label")
    plt.tight_layout()
    plt.savefig(save_path, dpi=300)
    plt.close()