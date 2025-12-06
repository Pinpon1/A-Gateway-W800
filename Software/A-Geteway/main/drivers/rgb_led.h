#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color_t;

/* 简单几种灯效，后面你可以自己扩展 */
typedef enum {
    RGB_EFFECT_NONE = 0,           /* 常亮 / 常灭 */
    RGB_EFFECT_BLINK,              /* 闪烁        */
    RGB_EFFECT_BREATH,             /* 呼吸灯      */
    RGB_EFFECT_ERROR_FAST_BLINK,   /* 错误快闪    */
} rgb_effect_t;

/**
 * @brief 初始化 RGB 逻辑：会调用 bsp_rgb_init()
 */
void rgb_led_init(void);

/**
 * @brief 设置“目标颜色”，会受全局亮度和灯效影响
 */
void rgb_led_set_color(rgb_color_t color);

/**
 * @brief 方便用 r/g/b 直接设置
 */
static inline void rgb_led_set_color_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    rgb_color_t c = { r, g, b };
    rgb_led_set_color(c);
}

/**
 * @brief 设置全局亮度 0~255（255 = 不衰减）
 */
void rgb_led_set_global_brightness(uint8_t brightness);

/**
 * @brief 设置灯效
 *
 * @param effect    灯效类型
 * @param base      灯效的基础颜色（比如呼吸灯的颜色）
 * @param period_ms 一个周期的时长（例如 1000ms）
 */
void rgb_led_set_effect(rgb_effect_t effect,
                        rgb_color_t   base,
                        uint16_t      period_ms);

/**
 * @brief 周期调用，用来更新灯效
 *
 * @param delta_ms 距离上一次调用经过的毫秒数
 *
 * 典型用法：在 10ms 或 20ms 的软定时器/任务里调用一次：
 *   rgb_led_task_tick(10);
 */
void rgb_led_task_tick(uint32_t delta_ms);

void rgb_led_set_error_mode(void);

#ifdef __cplusplus
}
#endif
