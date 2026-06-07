"""CAN bus traffic simulator and analyser for host-side development."""

from __future__ import annotations

import argparse
import json
import random
import time
from dataclasses import dataclass, asdict
from pathlib import Path


@dataclass
class CanMetrics:
    frames_received: int = 0
    frames_logged: int = 0
    frames_dropped: int = 0
    bus_errors: int = 0
    bus_load_pct: float = 0.0
    max_jitter_ms: float = 0.0


def simulate(duration_sec: float, bus_load: float, bitrate_mbps: float = 1.0) -> CanMetrics:
    rng = random.Random(42)
    metrics = CanMetrics()
    frame_interval = 1.0 / (bitrate_mbps * 1_000_000 / 128)  # ~128 bit average frame
    deadline = time.time() + duration_sec
    last = time.perf_counter()

    while time.time() < deadline:
        metrics.frames_received += 1
        if rng.random() <= bus_load:
            metrics.frames_logged += 1
        else:
            metrics.frames_dropped += 1

        if rng.random() < 0.001:
            metrics.bus_errors += 1

        now = time.perf_counter()
        jitter_ms = abs(now - last - frame_interval) * 1000
        metrics.max_jitter_ms = max(metrics.max_jitter_ms, jitter_ms)
        last = now
        time.sleep(frame_interval * (1.0 - bus_load * 0.2))

    metrics.bus_load_pct = round(metrics.frames_logged / max(metrics.frames_received, 1) * 100, 2)
    return metrics


def stress_test(hours: float, bus_load: float = 0.8) -> CanMetrics:
    return simulate(duration_sec=hours * 3600, bus_load=bus_load)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--duration", type=float, default=60.0)
    parser.add_argument("--load", type=float, default=0.8)
    parser.add_argument("--stress-test", action="store_true")
    parser.add_argument("--hours", type=float, default=24.0)
    parser.add_argument("--output", type=Path, default=Path("results/metrics.json"))
    args = parser.parse_args()

    metrics = stress_test(args.hours, args.load) if args.stress_test else simulate(args.duration, args.load)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(asdict(metrics), indent=2))
    print(json.dumps(asdict(metrics), indent=2))
