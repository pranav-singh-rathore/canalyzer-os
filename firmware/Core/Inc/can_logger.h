#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CAN_RX_QUEUE_DEPTH 256
#define CAN_MAX_DATA_LEN 8

typedef struct {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[CAN_MAX_DATA_LEN];
    uint32_t timestamp_ms;
} CanFrame;

typedef struct {
    uint32_t frames_received;
    uint32_t frames_logged;
    uint32_t frames_dropped;
    uint32_t bus_errors;
    float bus_load_pct;
} CanMetrics;

void can_logger_init(void);
bool can_logger_push(const CanFrame *frame);
void can_logger_get_metrics(CanMetrics *out);
