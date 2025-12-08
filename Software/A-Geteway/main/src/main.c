#include "geteway_include.h"

cht8305c_t app_cht8305c;

cht8305c_t sensor;
float t_c;
float rh;

int main(void)
{
#if APP_CFG_TEST_RGB
    /* RGB测试任务 */
    test_rgb_init();
#endif

    cht8305c_init(&app_cht8305c,CHT8305C_I2C_NAME,CHT8305C_I2C_ADDR_DEFAULT,100000);

    while (1)
    {
        int ret = cht8305c_read(&app_cht8305c, &t_c, &rh);

        if (ret == WM_ERR_SUCCESS)
        {
            wm_log_info("T=%.2f C, RH=%.1f %%", t_c, rh);
        }
        else
        {
            wm_log_error("cht8305c_read failed, ret=%d", ret);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

