#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "geteway_include.h"

/* 软硬模拟名称 */
#define CHT8305C_I2C_NAME_HW        "i2c"        /* 硬件 I2C 控制器名字 */
#define CHT8305C_I2C_NAME_SOFT      "i2c_soft"   /* 软件 I2C 控制器名字 */

//#define CHT8305C_USE_SOFT_I2C       1

/* 默认用哪个控制器名字：受 CHT8305C_USE_SOFT_I2C 影响 */
#ifdef CHT8305C_USE_SOFT_I2C
    #define CHT8305C_I2C_NAME       CHT8305C_I2C_NAME_SOFT
#else
    #define CHT8305C_I2C_NAME       CHT8305C_I2C_NAME_HW
#endif

/* CHT8305CADDR */
#define CHT8305C_I2C_ADDR_DEFAULT   0X40

/* CHT8305寄存器地址 */
#define CHT8305C_REG_DATA           0X00

/* 读数据转换时间跟AHT20类似 */
#define CHT8305C_CONVERT_DELAY_MS   20

/* -------- 仅在“硬件 I2C 模式”下使用的 IO 宏 --------
 * 软 I2C 模式下不会用到这些宏，IO 由设备表(i2c_soft0)决定
 */
#define CHT8305C_SDA        WM_GPIO_NUM_4
#define CHT8305C_SCL        WM_GPIO_NUM_1

/* PBx 对应的 PWM 复用功能，通常是 FUN2 */
#define CHT8305C_FUNC_SEL   WM_GPIO_IOMUX_FUN2


/* CHT8305C结构体 */
typedef struct 
{
    bsp_i2c_bus_t i2c;  /*底层I2C结构体*/
}cht8305c_t;

/**
 * @brief 初始化 CHT8305C
 * @param dev        传感器对象指针（外面定义一个变量传进来）
 * @param i2c_name   I2C 控制器名字，比如 "i2c"（走硬件）或 "i2c_soft0"（走软件）
 * @param addr_7bit  7bit 设备地址，一般 0x40
 * @param speed_hz   I2C 速度，比如 100000 / 400000
 */
int cht8305c_init(cht8305c_t *dev,
                      const char *i2c_name,
                      uint8_t    addr_7bit,
                      uint32_t   speed_hz);

/**
 * @brief 原始数据读取：读出 16bit 温度 + 16bit 湿度原始值
 */
int cht8305c_read_raw(cht8305c_t *dev,
                          uint16_t *raw_temp,
                          uint16_t *raw_rh);

/**
 * @brief 读出摄氏温度 / 相对湿度（已经转换好的物理量）
 */
int cht8305c_read(cht8305c_t *dev,
                      float *temp_c,
                      float *rh);

                      
                      
#ifdef __cplusplus
}
#endif
