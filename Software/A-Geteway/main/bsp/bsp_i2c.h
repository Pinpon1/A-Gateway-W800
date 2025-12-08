// app/drivers/bsp_i2c.h
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "geteway_include.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief I2C 运行时配置（地址 + 速率）
 */
typedef struct
{
    uint16_t addr;      /**< 7bit 从机地址 */
    uint32_t speed_hz;  /**< SCL 时钟，单位 Hz */
} bsp_i2c_cfg_t;

/**
 * @brief I2C 总线“盒子”
 *
 * dev：到底是硬件 I2C 还是软件 I2C，由 device_name 决定
 * cfg：当前这条从设备用的地址和速度
 * inited：是否已经初始化
 */
typedef struct
{
    wm_device_t   *dev;
    bsp_i2c_cfg_t  cfg;
    bool           inited;
} bsp_i2c_bus_t;

/**
 * @brief 初始化一条 I2C 总线
 *
 * @param bus             I2C 盒子指针
 * @param controller_name 在设备树里的 I2C 名字（比如 "i2c0" 或 "i2c_soft0"）
 * @param addr_7bit       7bit 从机地址（0x40 之类）
 * @param speed_hz        I2C 速率（建议用 WM_I2C_SPEED_STANDARD / FAST 等宏）
 *
 * @return WM_ERR_SUCCESS / 其他错误码
 */
int bsp_i2c_bus_init(bsp_i2c_bus_t *bus,
                     const char    *controller_name,
                     uint16_t       addr_7bit,
                     uint32_t       speed_hz);

/**
 * @brief 反初始化 I2C 总线
 */
void bsp_i2c_bus_deinit(bsp_i2c_bus_t *bus);

/**
 * @brief 通用 I2C 读：先写 sub_addr（可选），再读数据
 */
int bsp_i2c_read(bsp_i2c_bus_t *bus,
                 const void    *sub_addr,
                 uint32_t       sub_addr_size,
                 void          *buf,
                 uint32_t       size);

/**
 * @brief 通用 I2C 写：先写 sub_addr，再写数据
 */
int bsp_i2c_write(bsp_i2c_bus_t *bus,
                  const void    *sub_addr,
                  uint32_t       sub_addr_size,
                  const void    *data,
                  uint32_t       size);

/**
 * @brief 8bit 寄存器读封装
 */
static inline int bsp_i2c_read_reg8(bsp_i2c_bus_t *bus,
                                    uint8_t        reg,
                                    void          *buf,
                                    uint32_t       size)
{
    return bsp_i2c_read(bus, &reg, 1, buf, size);
}

/**
 * @brief 8bit 寄存器写封装
 */
static inline int bsp_i2c_write_reg8(bsp_i2c_bus_t *bus,
                                     uint8_t        reg,
                                     const void    *data,
                                     uint32_t       size)
{
    return bsp_i2c_write(bus, &reg, 1, data, size);
}

#ifdef __cplusplus
}
#endif
