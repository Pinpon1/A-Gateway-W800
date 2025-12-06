#pragma once

#include <stdint.h>

typedef enum {
    RGB_SYS_STATE_IDLE = 0,       // 闲置 / 未启动
    RGB_SYS_STATE_WIFI_CONFIG,    // 配网中
    RGB_SYS_STATE_WIFI_OK,        // WiFi 正常
    RGB_SYS_STATE_ERROR,          // 系统错误
} rgb_sys_state_t;

/* 初始化（里面会调用 rgb_led_init） */
void rgb_status_init(void);

/* 设置当前系统状态（外部直接调用这个就行） */
void rgb_status_set_state(rgb_sys_state_t state);

/* 周期调用，用来驱动灯效时间演进
 * 在有 RTOS 的情况下，可以在任务里每 10ms 调一次
 */
void rgb_status_task_tick(uint32_t delta_ms);
