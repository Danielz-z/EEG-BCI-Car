from __future__ import annotations

import time
import serial


class BluetoothController:
    def __init__(self, port: str, baudrate: int = 9600, timeout: int = 1):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.ser = None

    def connect(self):
        if self.ser is None or not self.ser.is_open:
            self.ser = serial.Serial(self.port, self.baudrate, timeout=self.timeout)

    def close(self):
        if self.ser is not None and self.ser.is_open:
            self.ser.close()

    def send(self, command: str) -> bool:
        try:
            self.connect()
            self.ser.write(command.encode())
            time.sleep(1)
            _ = self.ser.readline()
            return True
        except serial.SerialException:
            return False
        except Exception:
            return False