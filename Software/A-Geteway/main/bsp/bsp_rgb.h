#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板载 RGB（三个 PWM 通道 + IO 复用）
 */
void bsp_rgb_init(void);

/**
 * @brief 设置 RGB 亮度 0~255
 *        0   = 关灯
 *        255 = 该颜色最亮
 */
void bsp_rgb_set_rgb8(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief 关灯
 */
void bsp_rgb_off(void);

#ifdef __cplusplus
}
#endif
