#pragma once

/* c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* hal */
#include "wm_log.h"
#include "wm_drv_i2c.h"
#include "wm_dt_hw.h"

/* Middlewares */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* bsp */
#include "bsp_rgb.h"
#include "bsp_i2c.h"

/* Drivers */
#include "cht8305c.h"
#include "rgb_led.h"

/* APP */
#include "app_main.h"
#include "rgb_status.h" 

/* test */
#include "test_config.h"

#if APP_CFG_TEST_RGB
#include "test_rgb.h"
#endif



