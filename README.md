# EEG-BCI-Car

An EEG-based real-time brain-computer interface system for emergency vehicle takeover, integrating model training, offline inference, embedded hardware control, and GUI deployment.

---
## Highlights

- Real-time EEG-based intention recognition system
- End-to-end pipeline from signal to hardware control
- Integrated deep learning + embedded system deployment
- Award-winning project (CPWC, National Competition)

---
## Overview

This project explores how electroencephalography (EEG) signals can be translated into actionable driving commands for emergency takeover scenarios. The system is designed as an end-to-end pipeline that starts from EEG signal processing and feature extraction, proceeds through deep learning-based classification, and finally drives an embedded vehicle platform through Bluetooth communication.

The goal is to enable rapid and reliable recognition of driver emergency intentions, including:
- deceleration
- left lane change
- right lane change
- invalid command rejection

This repository contains:
- **model training** code for EEG classification
- **offline inference** code for prediction without hardware
- **real-time inference and GUI** for live deployment
- **STM32-based vehicle control** code for the model car

---

## Project Motivation

In safety-critical autonomous driving scenarios, delayed human intervention can lead to severe consequences. This project investigates whether EEG signals can be used as an additional control channel for emergency takeover, allowing a system to detect driver intention earlier than traditional manual response alone.

The core idea is:

**EEG signal → feature extraction → neural decoding → command decision → vehicle control**

This work combines signal processing, machine learning, real-time software, and embedded system integration into one complete prototype.

---

## System Architecture

The full system consists of four main layers:

1. **Training Layer**
   - EEG feature dataset construction
   - BP / LSTM / GRU / SVM / Logistic Regression baselines
   - model training and evaluation

2. **Inference Layer**
   - offline prediction using saved models
   - reusable prediction interface for validation

3. **Real-Time Layer**
   - live CSV-based EEG stream reading
   - feature extraction and sequence generation
   - model prediction and confidence estimation
   - GUI visualization and decision pipeline

4. **Hardware Layer**
   - Bluetooth command transmission
   - STM32-based model car control
   - lane-level action execution

A simplified data flow is:

```text
EEG Data
   ↓
Signal Processing / Feature Extraction
   ↓
LSTM / GRU / Baseline Models
   ↓
Decision Logic
   ↓
Bluetooth Communication
   ↓
STM32 Vehicle Control
```

---

## Repository Structure

```bash
EEG-BCI-Car/
├── hardware/                      # Embedded system
│   └── stm32/                    # STM32-based vehicle control code
│
├── training/                     # Model training and evaluation
│   ├── models/                  # Model definitions (BP / LSTM / GRU / etc.)
│   ├── outputs/                 # Saved models and figures
│   ├── config.py
│   ├── data_utils.py
│   ├── evaluate.py
│   ├── requirements.txt
│   └── train.py
│
├── inference/                    # Offline inference
│   ├── __init__.py
│   ├── config.py
│   ├── predictor.py
│   └── run_inference.py
│
├── realtime/                     # Real-time BCI system
│   ├── __init__.py
│   ├── main.py                  # Entry point for real-time system
│   ├── config.py
│   ├── control/                # Decision logic and command handling
│   ├── data/                   # Data reading and preprocessing
│   ├── hardware/               # Bluetooth communication
│   ├── model/                  # Model loading and prediction
│   ├── signal/                 # Signal processing and feature extraction
│   └── ui/                     # PyQt GUI interface
│
├── data/                        # Dataset and data description
├── demo/                        # Demo videos or GIFs
├── docs/                        # Figures and documentation
├── .gitignore
└── README.md
```

---

## Core Features

### 1. EEG Feature-Based Intention Recognition

The system uses EEG-derived features, including:
- raw signal statistics
- attention-related indicators
- frequency-domain descriptors
- time-domain summary features
- sequence-based temporal modeling

### 2. Multiple Baseline Models

The training pipeline supports several classifiers:
- BP neural network
- LSTM
- GRU
- SVM
- Logistic Regression

This makes it possible to compare classical machine learning methods with temporal neural models under a unified pipeline.

### 3. Real-Time Inference Pipeline

The real-time module supports:
- reading new EEG samples continuously
- extracting structured features
- building temporal sequences for inference
- predicting driver intention with confidence scores
- applying safety rules before sending commands

### 4. Embedded Vehicle Control

Predicted commands are transmitted through Bluetooth to an STM32-based vehicle platform. The controller supports command constraints such as:
- lane boundary checking
- cooldown between repeated lane changes
- cooldown for deceleration commands
- invalid-command rejection

### 5. Interactive GUI

A PyQt5-based interface is provided for deployment and demonstration, including:
- EEG signal visualization
- confidence trend display
- prediction label display
- lane state display
- EEG band power visualization
- start / stop takeover control


---
## Performance Summary

| Class            | Accuracy | Precision | Recall | Specificity |
|------------------|---------|----------|--------|------------|
| Decelerate       | 0.9561 | 0.8387 | 0.8261 | 0.9316 |
| LaneChange Left  | 0.8926 | 0.8339 | 0.8383 | 0.9307 |
| LaneChange Right | 0.8821 | 0.8358 | 0.8497 | 0.9274 |
| Invalid Command  | 0.9056 | 0.8904 | 0.8722 | 0.9877 |

The model demonstrates stable multi-class classification performance, 
with strong robustness in rejecting invalid commands.

---

## Awards
This project has been recognized in both international and national competitions:

- 🥇 World Cup Award, 13th Cloud Programming Grand Prix World Cup (CPWC),  
  Forum8 Design Festival, Japan, 2025

- 🥉 Third Prize, National Transportation Technology Competition, 2025

---
## Supported Commands

| Class | Meaning          |
|------|------------------|
| 0 | Decelerate       |
| 1 | LaneChange Left  |
| 2 | LaneChange Right |
| 3 | Invalid Command  |

At the hardware layer, these are mapped to compact Bluetooth commands for the vehicle controller.

---

## Training

The training/ module provides a unified training entry for multiple models.

### Example: Train an LSTM model

```bash
python training/train.py --data_path data/train.xlsx --model lstm --num_classes 4 --input_dim 21 --time_steps 2 --epochs 50 --batch_size 32
```

### Example: Train a BP baseline

```bash
python training/train.py --data_path data/train.xlsx --model bp --num_classes 4 --input_dim 21 --epochs 50 --batch_size 32
```

### Example: Train an SVM baseline

```bash
python training/train.py --data_path data/train.xlsx --model svm --num_classes 4 --input_dim 21
```

### Training outputs

The training module saves:

- trained model files
- training curves
- confusion matrix figures

---
## Offline Inference

The inference/ module is used for prediction without GUI or hardware.

### Run offline inference

```bash
python inference/run_inference.py
```

This is useful for:

- checking whether a saved model works correctly
- validating feature extraction
- debugging deployment without launching the real-time GUI

---

## Real-Time System

The realtime/ module contains the deployed BCI pipeline.

### Launch the real-time system

```bash
python -m realtime.main
```

Before running, configure the following in realtime/config.py:

- model_path
- csv_path
- serial_port

### Real-time workflow
1. read latest EEG samples from CSV
2. extract time/frequency features
3. assemble model input features
4. create temporal sequence
5. run neural prediction
6. estimate confidence
7. apply control logic
8. send valid command to the car
9. update GUI

---

## Hardware

The hardware/stm32/ folder contains the embedded code for the model car.

Its responsibilities include:

- receiving commands through Bluetooth
- interpreting control actions
- executing vehicle behaviors on the STM32 platform

This separation keeps hardware logic independent from the Python-side EEG inference pipeline.

---
## Input Data Specification

The project expects EEG-related feature tables with a label column named:

`Distraction`

Typical input features include:
- RawData
- Attention
- Delta
- HighAlpha
- HighBeta
- engineered feature columns
- time-domain features such as mean/std/rms
- frequency-domain features such as dominant frequency and PSD statistics

If your dataset uses a different label column, change the corresponding config or CLI argument.

---

## Environment Setup

Install dependencies for training:

```bash
pip install -r training/requirements.txt
```

For the real-time GUI, you may also need:

```bash
pip install pyqt5 pyserial scipy matplotlib pandas tensorflow scikit-learn
```

---
## Recommended Usage Order

For a clean reproduction workflow:

1. prepare or place dataset in data/
2. train a model in training/
3. test offline prediction in inference/
4. configure model path / CSV path / serial port
5. launch the GUI in realtime/
6. connect Bluetooth hardware and run the vehicle demo

---

## Current Limitations

This repository is a research and prototype system rather than a production-ready driving platform.

Known limitations include:

- dependence on precomputed CSV-based EEG input rather than direct device SDK integration
- limited command vocabulary
- dataset-specific feature structure
- hardware and serial settings that must be adapted for each environment
- real-time robustness still dependent on signal quality and upstream data stability

---

## Future Work

Possible next steps include:

- direct live EEG device integration
- stronger temporal modeling and subject generalization
- improved confidence calibration and rejection strategy
- multimodal fusion with eye tracking or behavioral signals
- more robust real-time deployment on edge hardware
- closed-loop evaluation under more realistic takeover scenarios

---

## Project Value

This repository is intended to demonstrate:

- EEG signal understanding
- time-series modeling with deep learning
- real-time system design
- human-machine interaction in safety-critical scenarios
- embedded deployment and hardware integration

It is not only a model training repository, but a full-stack prototype spanning signal processing, machine learning, real-time software, and embedded control.

---

## System Overview

### Automatic Driving Levels

![Automatic Driving Levels](pics/Automaticdrivingsystemclassification.png)

### System Architecture

![BCI Vehicle System](pics/BCIVehicleSystem.png)

---

## Data Collection & Experimental Setup

### EEG Signal Collection

![EEG Signal Collection](pics/EEGSignalCollection.png)

### Driving Simulator Setup

![Driving Simulator](pics/UCwinRoadsoftwaredrivingsimulator.png)

### Driving Data Features

![Driving Data](pics/DrivingPerformanceDataCollection.png)

---

## Methodology

### Project Technology Roadmap

![Technology Roadmap](pics/ProjectTechnologyRoadmap.png)

### Feature Extraction & EEG Analysis

![EEG Features](pics/LSTM.png)

### Experimental Workflow

![Workflow](pics/Workflow.png)

---

## Takeover Mechanism

### Takeover Methods Comparison

![Takeover Comparison](pics/TakeoverMethodsComparison.png)

### Automatic Driving Failure Scenario

![Failure Scenario](pics/Experimentalprocessofautomaticdrivingfailuretakeoverbasedondrivingsimulator.png)

---

## Data

### mindwave.csv

This file contains EEG data collected from the MindWave device.

Main content:
- Raw EEG signal (RawData)
- Attention value
- Some frequency-related features (Delta, Alpha, Beta, etc.)

Usage:
- Used as input data for model training and prediction


### mindwave_with_timestamps.csv

This file is similar to `mindwave.csv`, but includes time information.

Additional column:
- timestamp

Usage:
- Used when time sequence is needed (e.g., LSTM model)
- Helps simulate real-time data


### driving simulator.csv

This file contains driving behavior data.

Main content:
- Driving actions (e.g., lane change, deceleration)
- Labels for each action

Usage:
- Used as ground truth labels
- Helps train the model to map EEG signals to driving commands

---

## Notes

- The model combines EEG data and driving labels for training
- CSV files are also used to simulate real-time input in the system
---

## Demo

A demonstration of the real-time EEG-based control system:

![EEG Control Demo](./demo/videoexhibition.gif)

---

## Documentation

Place system figures and supplementary materials in: docs/

Recommended contents:
- Program concept and Program application
- User manual

---

## License

This project is released under the MIT License.

---

## Contact

For academic or project-related communication, please open an issue on this repository.

---