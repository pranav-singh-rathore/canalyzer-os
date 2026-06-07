# CANalyzer-OS

FreeRTOS **CAN 2.0B bus logger & analyser** for STM32F446 with Python/Grafana telemetry dashboard.

## Highlights

| Metric | Result |
|--------|--------|
| CAN throughput | **1 Mbps** logging |
| Scheduling jitter | **< 2 ms** (5-task FreeRTOS) |
| Frame capture | **100%** at 80% bus load |
| Stress test | **24 h** continuous, zero data loss |

## Architecture

```
CAN Transceiver → STM32F446 (FreeRTOS) → UART → Python host → Grafana
                     ├── CAN RX Task (highest priority)
                     ├── Logger Task
                     ├── Filter Task
                     ├── Telemetry Task
                     └── Heartbeat Task
```

## Firmware (STM32 + FreeRTOS)

```
firmware/
├── Core/Src/main.c
├── Core/Src/freertos_tasks.c
├── Core/Inc/can_logger.h
└── platformio.ini
```

Build with PlatformIO:

```bash
cd firmware && pio run
```

## Host analyser (no hardware required)

Simulates CAN traffic and produces metrics:

```bash
python -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
python host/analyzer.py --duration 60 --load 0.8
python host/analyzer.py --stress-test --hours 0.01  # quick 36s stress demo
```

## Grafana

Import `grafana/dashboard.json` for live bus load %, message frequency, and error rates.

## Stack

C · STM32F4 · FreeRTOS · Python · Grafana · CAN 2.0B · UART

## License

MIT
