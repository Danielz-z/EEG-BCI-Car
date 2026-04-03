from __future__ import annotations

import time


class CommandController:
    def __init__(self, cfg, bt_controller):
        self.cfg = cfg
        self.bt = bt_controller
        self.current_lane = cfg.initial_lane
        self.last_slow_down_time = 0.0
        self.last_lane_change_time = 0.0
        self.last_sent_command_name = None
        self.last_sent_command_time = 0.0
        self.send_commands = cfg.enable_send_commands

    def set_send_enabled(self, enabled: bool):
        self.send_commands = enabled

    def decide_command(self, pred_label: str, confidence: float):
        if confidence < self.cfg.confidence_threshold:
            return "InvalidCommand"

        if pred_label == "LaneChangeLeft" and confidence >= self.cfg.left_lane_change_threshold:
            return "LaneChangeLeft"

        if pred_label == "LaneChangeRight" and confidence >= self.cfg.right_lane_change_threshold:
            return "LaneChangeRight"

        if pred_label == "Decelerate" and confidence >= self.cfg.slow_down_threshold:
            return "Decelerate"

        return "InvalidCommand"

    def execute_command(self, command: str):
        now = time.time()

        if not self.send_commands:
            return False

        if command == "LaneChangeLeft":
            if self.current_lane > self.cfg.min_lane and now - self.last_lane_change_time > 5:
                ok = self.bt.send(self.cfg.command_mapping[command])
                if ok:
                    self.current_lane -= 1
                    self.last_lane_change_time = now
                    self.last_sent_command_name = command
                    self.last_sent_command_time = now
                return ok
            return False

        if command == "LaneChangeRight":
            if self.current_lane < self.cfg.max_lane and now - self.last_lane_change_time > 5:
                ok = self.bt.send(self.cfg.command_mapping[command])
                if ok:
                    self.current_lane += 1
                    self.last_lane_change_time = now
                    self.last_sent_command_name = command
                    self.last_sent_command_time = now
                return ok
            return False

        if command == "Decelerate":
            if now - self.last_slow_down_time > 2:
                ok = self.bt.send(self.cfg.command_mapping[command])
                if ok:
                    self.last_slow_down_time = now
                    self.last_sent_command_name = command
                    self.last_sent_command_time = now
                return ok
            return False

        return False