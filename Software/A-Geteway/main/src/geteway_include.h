#pragma once

/* c */
#include <stdio.h>
#include <stdlib.h>

/* hal */
#include "wm_log.h"

/* Middlewares */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* bsp */
#include "bsp_rgb.h"

/* APP */
#include "app_main.h"
#include "rgb_status.h" 

/* test */
#include "test_config.h"

#if APP_CFG_TEST_RGB
#include "test_rgb.h"
#endif



