#include "test_config.h"

#if APP_CFG_TEST_RGB     /* 如果没开测试，这个文件等于空 */

#include "rgb_led.h"
#include "geteway_include.h"

#define LOG_TAG "test_rgb" 

#define RGB_TEST_TASK_STACK_DEPTH  512
#define RGB_TEST_TASK_PRIO         5

/* 呼吸灯测试任务 */
static void rgb_test_task(void *pvParameters)
{
    (void)pvParameters;

    rgb_led_init();

    //rgb_led_set_error_mode();

    rgb_color_t colors[3] = {
        {255,0,0}, /* 红 */
        {0,255,0}, /* 绿 */
        {0,0,255}, /* 蓝 */
    };

    const TickType_t delay_tiacks    = pdMS_TO_TICKS(10);
    const uint16_t   period_ms       = 2000;                    /* 一个周期2000MS    */
    const uint16_t   step_ms         = 10;                      /* 每次tick推进10ms  */
    const uint16_t   steps_per_cycle = period_ms / step_ms;     /* 200次            */

    for(;;)
    {
        for(uint8_t i=0; i<3;i++)
        {
            /* 设置当前颜色的呼吸效果 */
            rgb_led_set_effect(RGB_EFFECT_BREATH,colors[i],period_ms);

            wm_log_raw_info(">>> RGB switch %u <<<\r\n", i);

            for(uint16_t step = 0; step<steps_per_cycle; step++)
            {
                rgb_led_task_tick(step_ms);
                vTaskDelay(delay_tiacks);
            }
        }
    }
    
}

void test_rgb_init(void)
{
    xTaskCreate(
        rgb_test_task,
        "rgb_test",
        RGB_TEST_TASK_STACK_DEPTH,
        NULL,
        RGB_TEST_TASK_PRIO,
        NULL
    );
}

#endif /* APP_CFG_TEST_RGB */
