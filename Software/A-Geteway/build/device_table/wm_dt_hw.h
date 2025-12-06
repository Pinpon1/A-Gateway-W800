/**
 * @file wm_dt_hw.h
 *
 * @brief Hardware Info Module
 *
 */

/**
 *  Copyright 2022-2024 Beijing WinnerMicroelectronics Co.,Ltd.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef __WM_DT_HW_H__
#define __WM_DT_HW_H__

#include "wmsdk_config.h"
#include "wm_types.h"
#include "wm_soc_cfgs.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef struct {
    uint8_t init_level;
    uint8_t init_priority;
} wm_dt_hw_init_cfg_t;

typedef struct {
    uint8_t irq_num;  /**< @ref wm_irq_no_t */
    uint8_t irq_priority;
} wm_dt_hw_irq_cfg_t;

typedef struct {
    uint8_t pin_num;  /**< @ref wm_gpio_num_t */
    uint8_t pin_mux;  /**< @ref wm_gpio_pin_mux_t */
    uint8_t pin_dir;  /**< @ref wm_gpio_dir_t */
    uint8_t pin_pupd; /**< @ref wm_gpio_pupd_t */
} wm_dt_hw_pin_cfg_t;

typedef struct {
    wm_rcc_type_t type; /**< @ref wm_rcc_type_t */
    uint32_t clock;     /**< Define the clock for each of bus/controller, unit:Hz,
                         *   The range is same as that defined by \c wm_drv_rcc_config_clock()*/
} wm_dt_hw_rcc_cfg_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;
    uint32_t reg_base;
    uint8_t rcc_cfg_count;
    wm_dt_hw_rcc_cfg_t *rcc_cfg;
} wm_dt_hw_rcc_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;
} wm_dt_hw_dma_t;

typedef struct {
    wm_gpio_num_t pin_num;
    wm_gpio_pin_mux_t pin_mux;
    wm_gpio_dir_t pin_dir;
    wm_gpio_pupd_t pin_pupd;
    wm_gpio_intr_mode_t int_mode;
} wm_dt_hw_gpio_cfg_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint8_t gpio_cfg_count;
    wm_dt_hw_gpio_cfg_t *gpio_cfg;
} wm_dt_hw_gpio_t;

typedef struct {
    int baudrate;      /**< @ref wm_uart_baudrate_t */
    uint8_t parity;    /**< @ref wm_uart_parity_t */
    uint8_t stop_bits; /**< @ref wm_uart_stop_bits_t */
    uint8_t data_bits; /**< @ref wm_uart_data_bits_t */
    uint8_t flow_ctrl; /**< @ref wm_uart_flowctrl_t */
} wm_dt_hw_uart_cfg_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;

    wm_dt_hw_uart_cfg_t uart_cfg;

    uint8_t pin_cfg_count;
    wm_dt_hw_pin_cfg_t *pin_cfg;

    char *dma_device_name;
    char *rcc_device_name;
} wm_dt_hw_uart_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;

    uint8_t pin_cfg_count;
    wm_dt_hw_pin_cfg_t *pin_cfg;

    char *dma_device_name;
} wm_dt_hw_pwm_t;

typedef struct {
    bool quad_spi;
} wm_dt_hw_flash_cfg_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;

    wm_dt_hw_flash_cfg_t flash_cfg;
} wm_dt_hw_iflash_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;
} wm_dt_hw_rtc_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;

    wm_pmu_clock_source_t clk_src;
} wm_dt_hw_pmu_t;

typedef struct {
    uint32_t counter_value;
} wm_dt_hw_wdt_cfg_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;

    wm_dt_hw_wdt_cfg_t wdt_cfg;
} wm_dt_hw_wdt_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;

    char *rcc_device_name;
} wm_dt_hw_crc_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;

    char *rcc_device_name;
} wm_dt_hw_hash_t;

typedef struct {
    wm_dt_hw_init_cfg_t init_cfg;

    uint32_t reg_base;
    wm_dt_hw_irq_cfg_t irq_cfg;

    char *rcc_device_name;
} wm_dt_hw_rng_t;

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* __WM_DT_HW_H__ */