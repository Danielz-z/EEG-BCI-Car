from __future__ import annotations

from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QFrame
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QFont, QPalette
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


class MainWindow(QWidget):
    def __init__(self, cfg, pipeline):
        super().__init__()
        self.cfg = cfg
        self.pipeline = pipeline

        self.eeg_bands = ['Delta', 'Theta', 'Low Alpha', 'High Alpha', 'Low Beta', 'High Beta', 'Low Gamma', 'High Gamma']
        self.eeg_band_values = [0] * 8

        self.signal_values = []
        self.attention_values = []
        self.confidence_values = []
        self.time_points = []

        self.setWindowTitle("Brain-Controlled Emergency Takeover System")
        self.setFixedSize(1400, 800)

        self.setAutoFillBackground(True)
        palette = self.palette()
        palette.setColor(QPalette.Window, Qt.white)
        self.setPalette(palette)

        self.init_ui()

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_interface)
        self.timer.start(cfg.timer_interval_ms)

    def bordered_widget(self, inner_widget, border_color):
        frame = QFrame()
        frame.setStyleSheet(f"border: 3px solid {border_color}; border-radius: 6px;")
        layout = QVBoxLayout(frame)
        layout.setContentsMargins(2, 2, 2, 2)
        layout.addWidget(inner_widget)
        return frame

    def neon_frame(self, widget, border_color):
        widget.setStyleSheet(f"""
            background-color: #121625;
            border: 2px solid {border_color};
            border-radius: 10px;
        """)

    def init_ui(self):
        main_layout = QHBoxLayout()
        CYBER_BORDER = '#0099FF'

        left_layout = QVBoxLayout()

        self.car_image_label = QLabel()
        self.car_image_label.setAlignment(Qt.AlignCenter)
        self.car_image_label.setStyleSheet("background-color: #FFFFFF;")
        left_layout.addWidget(self.bordered_widget(self.car_image_label, CYBER_BORDER))

        self.fig1 = Figure(figsize=(6, 3), facecolor='white')
        self.ax1 = self.fig1.add_subplot(111)
        self.line1, = self.ax1.plot([], [], label='Signal', color='#FF4B91')
        self.line2, = self.ax1.plot([], [], label='Attention', color='#00E0FF')
        self.ax1.set_facecolor('#FFFFFF')
        self.ax1.tick_params(colors='black')
        self.ax1.set_title('EEG Signal', color='black')
        self.ax1.set_xlabel('Time', color='black')
        self.ax1.set_ylabel('Value', color='black')
        self.ax1.legend(facecolor='#FFFFFF', edgecolor='black', labelcolor='black')
        self.canvas1 = FigureCanvas(self.fig1)
        self.neon_frame(self.canvas1, '#00FFFF')
        left_layout.addWidget(self.bordered_widget(self.canvas1, CYBER_BORDER))

        center_layout = QVBoxLayout()

        self.fig2 = Figure(figsize=(6, 2.5), facecolor='white')
        self.ax2 = self.fig2.add_subplot(111)
        self.fig2.subplots_adjust(bottom=0.25)
        self.confidence_line, = self.ax2.plot([], [], label='Confidence', color='#FFA500')
        self.ax2.set_facecolor('#FFFFFF')
        self.ax2.tick_params(colors='black')
        self.ax2.set_title('Confidence Trend', color='black')
        self.ax2.set_xlabel('Time', color='black')
        self.ax2.set_ylabel('Confidence', color='black')
        self.ax2.legend(facecolor='#FFFFFF', edgecolor='black', labelcolor='black')
        self.canvas2 = FigureCanvas(self.fig2)
        self.neon_frame(self.canvas2, '#FF00FF')
        center_layout.addWidget(self.bordered_widget(self.canvas2, CYBER_BORDER))

        self.lane_label = QLabel("Lane: 3")
        self.lane_label.setFont(QFont("Arial", 22, QFont.Bold))
        self.lane_label.setAlignment(Qt.AlignCenter)
        self.lane_label.setStyleSheet("color: #333333; background-color: #FFFFFF; padding: 8px;")
        center_layout.addWidget(self.bordered_widget(self.lane_label, CYBER_BORDER))

        self.prediction_label = QLabel("Prediction")
        self.prediction_label.setFont(QFont("Arial", 28, QFont.Bold))
        self.prediction_label.setAlignment(Qt.AlignCenter)
        self.prediction_label.setStyleSheet("color: #00FFFF; background-color: #FFFFFF; padding: 10px;")
        center_layout.addWidget(self.bordered_widget(self.prediction_label, CYBER_BORDER))

        right_layout = QVBoxLayout()

        self.equipment_image_label = QLabel()
        self.equipment_image_label.setAlignment(Qt.AlignCenter)
        self.equipment_image_label.setStyleSheet("background-color: #FFFFFF;")
        right_layout.addWidget(self.bordered_widget(self.equipment_image_label, CYBER_BORDER))

        self.radar_fig = Figure(figsize=(3.5, 3.4), facecolor='white')
        self.radar_ax = self.radar_fig.add_subplot(111)
        self.radar_ax.set_facecolor('#FFFFFF')
        self.radar_ax.tick_params(colors='black')
        self.radar_ax.set_title('EEG Band Power', color='black')
        self.radar_ax.set_ylabel('Power Value', color='black')
        self.radar_ax.set_xticks(range(8))
        self.radar_ax.set_xticklabels(self.eeg_bands, rotation=30, ha='right', fontsize=8, color='black')
        self.radar_bars = self.radar_ax.bar(range(8), self.eeg_band_values, color='#7CFFCB')
        self.radar_canvas = FigureCanvas(self.radar_fig)
        self.neon_frame(self.radar_canvas, '#ADFF2F')
        right_layout.addWidget(self.bordered_widget(self.radar_canvas, CYBER_BORDER))

        self.start_button = QPushButton("▶ Start Takeover")
        self.start_button.setStyleSheet("""
            QPushButton {
                background-color: #00E0FF;
                color: black;
                font-size: 24px;
                padding: 20px;
                border-radius: 10px;
            }
            QPushButton:hover {
                background-color: #3CFFFB;
            }
        """)
        self.start_button.clicked.connect(self.start_sending)

        self.stop_button = QPushButton("■ Stop")
        self.stop_button.setStyleSheet("""
            QPushButton {
                background-color: #FF4B91;
                color: white;
                font-size: 24px;
                padding: 20px;
                border-radius: 10px;
            }
            QPushButton:hover {
                background-color: #FF80B3;
            }
        """)
        self.stop_button.clicked.connect(self.stop_sending)

        right_layout.addWidget(self.start_button)
        right_layout.addWidget(self.stop_button)

        main_layout.addLayout(left_layout, 2)
        main_layout.addLayout(center_layout, 2)
        main_layout.addLayout(right_layout, 1)
        self.setLayout(main_layout)

    def start_sending(self):
        self.pipeline.controller.set_send_enabled(True)

    def stop_sending(self):
        self.pipeline.controller.set_send_enabled(False)
        self.pipeline.bt.send(self.cfg.command_mapping["Stop"])

    def update_interface(self):
        result = self.pipeline.step()
        if result is None:
            return

        pred_label = result["pred_label"]
        confidence = result["confidence"]
        signal_value = result["signal_value"]
        attention_value = result["attention_value"]
        lane = result["lane"]
        eeg_bands = result.get("eeg_band_values", self.eeg_band_values)

        t = len(self.time_points)
        self.time_points.append(t)
        self.signal_values.append(signal_value)
        self.attention_values.append(attention_value)
        self.confidence_values.append(confidence)

        self.line1.set_data(self.time_points, self.signal_values)
        self.line2.set_data(self.time_points, self.attention_values)
        self.ax1.relim()
        self.ax1.autoscale_view()
        self.canvas1.draw()

        self.confidence_line.set_data(self.time_points, self.confidence_values)
        self.ax2.relim()
        self.ax2.autoscale_view()
        self.canvas2.draw()

        self.prediction_label.setText(f"{pred_label} ({confidence:.2f})")
        self.lane_label.setText(f"Lane: {lane}")

        for bar, val in zip(self.radar_bars, eeg_bands):
            bar.set_height(val)
        self.radar_ax.relim()
        self.radar_ax.autoscale_view()
        self.radar_canvas.draw()