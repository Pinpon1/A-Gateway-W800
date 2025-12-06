#include "rgb_status.h"
#include "rgb_led.h"

static rgb_sys_state_t s_cur_state = RGB_SYS_STATE_IDLE;

void rgb_status_init(void)
{
    rgb_led_init();
    rgb_status_set_state(RGB_SYS_STATE_IDLE);
}

void rgb_status_set_state(rgb_sys_state_t state)
{
    s_cur_state = state;

    switch (state) {
    case RGB_SYS_STATE_IDLE: {
        // 关灯或者弱白光
        rgb_led_set_effect(RGB_EFFECT_NONE, (rgb_color_t){0,0,0}, 0);
        break;
    }
    case RGB_SYS_STATE_WIFI_CONFIG: {
        // 蓝色呼吸：表示正在配网
        rgb_color_t blue = {0, 0, 255};
        rgb_led_set_effect(RGB_EFFECT_BREATH, blue, 2000);
        break;
    }
    case RGB_SYS_STATE_WIFI_OK: {
        // 绿色常亮：联网成功
        rgb_color_t green = {0, 255, 0};
        rgb_led_set_effect(RGB_EFFECT_NONE, green, 0);
        break;
    }
    case RGB_SYS_STATE_ERROR: {
        // 红色快闪：错误
        rgb_color_t red = {255, 0, 0};
        rgb_led_set_effect(RGB_EFFECT_BLINK, red, 400);
        break;
    }
    default:
        break;
    }
}

void rgb_status_task_tick(uint32_t delta_ms)
{
    // 目前只是简单透传给 rgb_led，将来要做更复杂状态机也在这里扩展
    rgb_led_task_tick(delta_ms);
}
