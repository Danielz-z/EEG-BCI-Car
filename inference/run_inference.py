from inference.config import InferenceConfig
from inference.predictor import OfflineInference


def main():
    cfg = InferenceConfig()
    runner = OfflineInference(cfg)
    result = runner.run_once()

    if result is None:
        print("Not enough feature history yet.")
        return

    print("Prediction Result")
    print(result)


if __name__ == "__main__":
    main()