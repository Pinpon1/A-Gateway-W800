#define LOG_TAG "bsp_rgb"

#include "bsp_rgb.h"

#include <stdbool.h>
#include <string.h>

#include "wm_error.h"
#include "wm_log.h"
#include "wm_drv_gpio.h"
#include "wm_drv_pwm.h"
#include "wm_w800_cfgs.h"


/* PB1 → R, PB0 → G, PB2 → B */
#define RGB_GPIO_R   WM_GPIO_NUM_17   /* PB1 */
#define RGB_GPIO_G   WM_GPIO_NUM_16   /* PB0 */
#define RGB_GPIO_B   WM_GPIO_NUM_18   /* PB2 */

/* 对应 PWM 通道：一般 PB0→CH0, PB1→CH1, PB2→CH2 */
#define RGB_PWM_CH_R   1U
#define RGB_PWM_CH_G   0U
#define RGB_PWM_CH_B   2U

/* PBx 对应的 PWM 复用功能，通常是 FUN2 */
#define RGB_PWM_FUNC_SEL   WM_GPIO_IOMUX_FUN2

/* 一个周期 256 个计数，方便 0~255 直接做占空比 */
/* API范围0-255                              */
#define RGB_PWM_PERIOD     (255U)
#define RGB_PWM_CLKDIV     (40U)   /* 40MHz / ((255+1)*40) ≈ 3.9kHz */

static wm_device_t *s_pwm_dev = NULL;
static bool         s_inited  = false;

/* ------------ 内部函数声明 ------------ */
static int  rgb_config_pinmux(void);
static int  rgb_pwm_channel_init(uint8_t channel);
static void rgb_set_channel(uint8_t channel, uint8_t level);

/* ------------ IO 复用配置 ------------ */
static int rgb_config_pinmux(void)
{
    int ret;

    ret = wm_drv_gpio_iomux_func_sel(RGB_GPIO_R, RGB_PWM_FUNC_SEL);
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("RGB R pin mux failed, ret=%d", ret);
        return ret;
    }

    ret = wm_drv_gpio_iomux_func_sel(RGB_GPIO_G, RGB_PWM_FUNC_SEL);
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("RGB G pin mux failed, ret=%d", ret);
        return ret;
    }

    ret = wm_drv_gpio_iomux_func_sel(RGB_GPIO_B, RGB_PWM_FUNC_SEL);
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("RGB B pin mux failed, ret=%d", ret);
        return ret;
    }

    return WM_ERR_SUCCESS;
}

/* ------------ 通道初始化：一次性用 channel_init 配齐 ------------ */
static int rgb_pwm_channel_init(uint8_t channel)
{
    wm_drv_pwm_channel_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.channel      = channel;              /* 0/1/2 */
    cfg.mode         = 0;                    /* 独立输出模式 WM_PWM_OUT_INDPT */
    cfg.int_type     = 0;                    /* 不用捕获/中断 */

    cfg.clkdiv       = RGB_PWM_CLKDIV;
    cfg.period_cycle = RGB_PWM_PERIOD;
    cfg.duty_cycle   = RGB_PWM_PERIOD;       /* 共阳极：默认全高电平 = 熄灭 */

    cfg.autoload     = true;
    cfg.period_int   = false;
    cfg.inverse      = false;
    cfg.output_high  = false;

    int ret = wm_drv_pwm_channel_init(s_pwm_dev, &cfg);
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("RGB ch%u channel_init failed, ret=%d", channel, ret);
    }
    return ret;
}

/* ------------ 公共接口 ------------ */
void bsp_rgb_init(void)
{
    int ret;

    if (s_inited) {
        return;
    }

    ret = rgb_config_pinmux();
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("rgb_config_pinmux failed, ret=%d", ret);
        return;
    }

    s_pwm_dev = wm_drv_pwm_init("pwm");
    if (s_pwm_dev == NULL) {
        wm_log_error("wm_drv_pwm_init failed");
        return;
    }

    ret  = rgb_pwm_channel_init(RGB_PWM_CH_R);
    ret |= rgb_pwm_channel_init(RGB_PWM_CH_G);
    ret |= rgb_pwm_channel_init(RGB_PWM_CH_B);
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("rgb_pwm_channel_init failed, ret=%d", ret);
        return;
    }

    ret = wm_drv_pwm_channels_start(s_pwm_dev);
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("wm_drv_pwm_channels_start failed, ret=%d", ret);
        return;
    }

    s_inited = true;
    bsp_rgb_off();
}

/* level: 0~255, 0=关灯, 255=最亮；
 * 共阳极 → duty = PERIOD - level
 */
static void rgb_set_channel(uint8_t channel, uint8_t level)
{
    if (!s_inited || s_pwm_dev == NULL) {
        return;
    }

    uint8_t duty = (uint8_t)(RGB_PWM_PERIOD - level);
    wm_drv_pwm_set_channel_duty(s_pwm_dev, channel, duty);
}

void bsp_rgb_set_rgb8(uint8_t r, uint8_t g, uint8_t b)
{
    rgb_set_channel(RGB_PWM_CH_R, r);
    rgb_set_channel(RGB_PWM_CH_G, g);
    rgb_set_channel(RGB_PWM_CH_B, b);
}

void bsp_rgb_off(void)
{
    bsp_rgb_set_rgb8(0, 0, 0);
}
