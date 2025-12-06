#include "rgb_led.h"
#include "bsp_rgb.h"
#include <string.h>   /* memset */

typedef struct {
    rgb_color_t   base_color;   /* 灯效的“基色”                       */   
    rgb_color_t   current_color;/* 当前实际输出的颜色（不含全局亮度）   */  
    rgb_effect_t  effect;       /* 当前灯效                           */ 
    uint16_t      period_ms;    /* 一个周期多久                       */  
    uint32_t      elapsed_ms;   /* 当前周期已经走了多少毫秒            */  
    uint8_t       brightness;   /* 全局亮度 0~255                    */   
    bool          initialized;
} rgb_led_state_t;

static rgb_led_state_t s_rgb;

/* 根据 current_color + brightness，把结果写到 BSP */
static void rgb_led_apply_output(void)
{
    if (!s_rgb.initialized) {
        return;
    }

    /* 全局亮度缩放：out = color * brightness / 255 */
    uint16_t r = (uint16_t)s_rgb.current_color.r * s_rgb.brightness / 255;
    uint16_t g = (uint16_t)s_rgb.current_color.g * s_rgb.brightness / 255;
    uint16_t b = (uint16_t)s_rgb.current_color.b * s_rgb.brightness / 255;

    bsp_rgb_set_rgb8((uint8_t)r, (uint8_t)g, (uint8_t)b);
}

/* 计算当前灯效下应该输出的颜色，填到 current_color */
static void rgb_led_update_effect_color(void)
{
    if (s_rgb.effect == RGB_EFFECT_NONE || s_rgb.period_ms == 0) {
        /* 无灯效：直接显示 base_color */
        s_rgb.current_color = s_rgb.base_color;
        return;
    }

    uint32_t t = s_rgb.elapsed_ms % s_rgb.period_ms;
    uint16_t half = s_rgb.period_ms / 2;

    rgb_color_t out = {0, 0, 0};

    switch (s_rgb.effect) {
    case RGB_EFFECT_BLINK:
        /* 前半周期亮，后半周期灭 */
        if (t < half) {
            out = s_rgb.base_color;
        } else {
            out.r = out.g = out.b = 0;
        }
        break;

    case RGB_EFFECT_BREATH: {
        /* 简单三角波：0 → 255 → 0 */
        uint16_t level;
        if (t < half) {
            level = (uint16_t)(t * 255 / half);
        } else {
            level = (uint16_t)((s_rgb.period_ms - t) * 255 / half);
        }

        out.r = (uint8_t)((uint16_t)s_rgb.base_color.r * level / 255);
        out.g = (uint8_t)((uint16_t)s_rgb.base_color.g * level / 255);
        out.b = (uint8_t)((uint16_t)s_rgb.base_color.b * level / 255);
        break;
    }
    case RGB_EFFECT_ERROR_FAST_BLINK:
    {
        uint16_t quarter = s_rgb.period_ms / 4;
        uint8_t  idx     = quarter;

        if(idx == 0 || idx == 2)
        {
            out = s_rgb.base_color;
        }
        else
        {
            out.r = out.g = out.b = 0;
        }

        break;
    }
    default:
        /* 预留：以后扩展更多灯效类型 */
        out = s_rgb.base_color;
        break;
    }

    s_rgb.current_color = out;
}

void rgb_led_init(void)
{
    memset(&s_rgb, 0, sizeof(s_rgb));
    s_rgb.brightness  = 255;     /* 默认最大亮度 */
    s_rgb.effect      = RGB_EFFECT_NONE;
    s_rgb.period_ms   = 0;
    s_rgb.initialized = true;

    bsp_rgb_init();
    bsp_rgb_off();
}

void rgb_led_set_color(rgb_color_t color)
{
    if (!s_rgb.initialized) {
        rgb_led_init();
    }

    s_rgb.base_color   = color;
    s_rgb.elapsed_ms   = 0;                /* 换颜色时重置周期 */
    s_rgb.effect       = RGB_EFFECT_NONE;  /* 默认不加灯效 */
    s_rgb.current_color = color;

    rgb_led_apply_output();
}

void rgb_led_set_global_brightness(uint8_t brightness)
{
    if (!s_rgb.initialized) {
        rgb_led_init();
    }

    s_rgb.brightness = brightness;
    /* 亮度变了，只需要重新应用一次当前颜色即可 */
    rgb_led_apply_output();
}

void rgb_led_set_effect(rgb_effect_t effect,
                        rgb_color_t   base,
                        uint16_t      period_ms)
{
    if (!s_rgb.initialized) {
        rgb_led_init();
    }

    s_rgb.effect      = effect;
    s_rgb.base_color  = base;
    s_rgb.period_ms   = period_ms ? period_ms : 1; /* 防止除零 */
    s_rgb.elapsed_ms  = 0;

    rgb_led_update_effect_color();
    rgb_led_apply_output();
}

void rgb_led_task_tick(uint32_t delta_ms)
{
    if (!s_rgb.initialized) {
        return;
    }

    if (s_rgb.effect == RGB_EFFECT_NONE || s_rgb.period_ms == 0) {
        return; /* 没有灯效就不用更新 */
    }

    s_rgb.elapsed_ms += delta_ms;

    rgb_led_update_effect_color();
    rgb_led_apply_output();
}

void rgb_led_set_error_mode(void)
{
    rgb_color_t red = {255, 0, 0};
    rgb_led_set_effect(RGB_EFFECT_ERROR_FAST_BLINK, red, 400); // 400ms 一个周期
}
