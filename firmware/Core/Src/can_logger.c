#include "can_logger.h"

#include <string.h>

static CanFrame rx_queue[CAN_RX_QUEUE_DEPTH];
static volatile uint16_t head = 0;
static volatile uint16_t tail = 0;
static CanMetrics metrics = {0};

void can_logger_init(void) {
    memset(&metrics, 0, sizeof(metrics));
    head = tail = 0;
}

bool can_logger_push(const CanFrame *frame) {
    metrics.frames_received++;
    uint16_t next = (head + 1) % CAN_RX_QUEUE_DEPTH;
    if (next == tail) {
        metrics.frames_dropped++;
        return false;
    }
    rx_queue[head] = *frame;
    head = next;
    metrics.frames_logged++;
    return true;
}

void can_logger_get_metrics(CanMetrics *out) {
    *out = metrics;
    out->bus_load_pct = (float)metrics.frames_logged / (float)(metrics.frames_received + 1) * 100.0f;
}
