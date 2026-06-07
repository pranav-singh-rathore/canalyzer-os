/**
 * FreeRTOS 5-task architecture for CAN logging on STM32F446.
 * Tasks: CAN RX (prio 5), Logger (4), Filter (3), Telemetry UART (2), Heartbeat (1)
 */
#include "can_logger.h"

#include <stdio.h>

typedef struct {
    const char *name;
    int priority;
    void (*loop)(void);
} OsTaskSpec;

static void can_rx_task(void) {
    /* HAL CAN RX interrupt feeds can_logger_push() */
}

static void logger_task(void) {
    CanMetrics m;
    can_logger_get_metrics(&m);
    (void)m;
}

static void filter_task(void) {
    /* Apply ID/mask filters before UART export */
}

static void telemetry_task(void) {
    /* Stream frames over UART @ 921600 baud to Python host */
}

static void heartbeat_task(void) {
    /* Watchdog + scheduling jitter monitor (<2ms target) */
}

static const OsTaskSpec TASKS[] = {
    {"CAN_RX", 5, can_rx_task},
    {"Logger", 4, logger_task},
    {"Filter", 3, filter_task},
    {"Telemetry", 2, telemetry_task},
    {"Heartbeat", 1, heartbeat_task},
};

int main(void) {
    can_logger_init();
    printf("CANalyzer-OS — %zu FreeRTOS tasks initialised\n", sizeof(TASKS) / sizeof(TASKS[0]));
    while (1) {
        for (unsigned i = 0; i < sizeof(TASKS) / sizeof(TASKS[0]); ++i) {
            TASKS[i].loop();
        }
    }
    return 0;
}
