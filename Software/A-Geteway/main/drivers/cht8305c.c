#include "geteway_include.h"

//#define LOG_TAG "CHT8305C"

#define CHT8305C_DEBUG_LOG   1

#if !defined(CHT8305C_USE_SOFT_I2C) || (CHT8305C_USE_SOFT_I2C == 0)

/* 复用IO初始化 */
static int cht8305c_config_pinmux(void)
{
    int ret;

    /* SDA: 输出 + 上拉 + 复用 */
    ret = wm_drv_gpio_set_dir(CHT8305C_SDA, WM_GPIO_DIR_OUTPUT);
    if (ret != WM_ERR_SUCCESS) {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("SDA pin dir failed, ret=%d", ret);
    #endif
        return ret;
    }

    ret = wm_drv_gpio_set_pullmode(CHT8305C_SDA, WM_GPIO_PULL_UP);
    if (ret != WM_ERR_SUCCESS) {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("SDA pin pull failed, ret=%d", ret);
    #endif
        return ret;
    }

    /* SCL: 输出 + 上拉 + 复用 */
    ret = wm_drv_gpio_set_dir(CHT8305C_SCL, WM_GPIO_DIR_OUTPUT);
    if (ret != WM_ERR_SUCCESS) {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("SCL pin dir failed, ret=%d", ret);
    #endif
        return ret;
    }

    ret = wm_drv_gpio_set_pullmode(CHT8305C_SCL, WM_GPIO_PULL_UP);
    if (ret != WM_ERR_SUCCESS) {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("SCL pin pull failed, ret=%d", ret);
    #endif
        return ret;
    }

    /* 复用到 I2C 功能 */
    ret = wm_drv_gpio_iomux_func_sel(CHT8305C_SDA, CHT8305C_FUNC_SEL);
    if (ret != WM_ERR_SUCCESS) {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("SDA pin mux failed, ret=%d", ret);
    #endif
        return ret;
    }

    ret = wm_drv_gpio_iomux_func_sel(CHT8305C_SCL, CHT8305C_FUNC_SEL);
    if (ret != WM_ERR_SUCCESS) {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("SCL pin mux failed, ret=%d", ret);
    #endif
        return ret;
    }
    return WM_ERR_SUCCESS;
}

#endif

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
                  uint32_t   speed_hz)
{
    /* 最终用哪个 I2C 控制器名字：传进来优先，否则用默认宏 */
    const char *ctrl_name = i2c_name ? i2c_name :CHT8305C_I2C_NAME;

    int ret;

    #if !defined(CHT8305C_USE_SOFT_I2C) || (CHT8305C_USE_SOFT_I2C == 0)
    /* 硬件 I2C 模式：自己配置 PA1/PA4 的复用功能 */
    ret = cht8305c_config_pinmux();
    if (ret != WM_ERR_SUCCESS) {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("CHT8305C pinmux failed, ret=%d", ret);
    #endif
        return ret;
    }
    #endif


    /* 检查空指针 */
    if(!dev || !ctrl_name)
    {
        #if CHT8305C_DEBUG_LOG        
        wm_log_error("CHT8305C init: invalid param dev=%p, ctrl=%p",
                     dev, ctrl_name);
        #endif
        
        return WM_ERR_INVALID_PARAM;
    }
    /* 先清除数据 */
    memset(dev,0,sizeof(*dev));

    /* 使用板极I2C初始化CHT8305C */
    ret = bsp_i2c_bus_init(&dev->i2c,
                           ctrl_name, 
                           addr_7bit,
                           speed_hz  
    );

    /* 检查是否初始化成功 */
    if(ret != WM_ERR_SUCCESS)
    {
        #if CHT8305C_DEBUG_LOG
        wm_log_error("CHT8305C i2c init failed, name=%s", ctrl_name);  
        #endif

        return ret;
    }

    /* 自检一次 */
    #if CHT8305C_DEBUG_LOG
    wm_log_info("CHT8305C init ok, addr=0x%02X, speed=%u Hz",
                           addr_7bit, (unsigned int)speed_hz);
    #endif                       

    return WM_ERR_SUCCESS;    
}


/**
 * @brief 原始数据读取：读出 16bit 温度 + 16bit 湿度原始值
 */
int cht8305c_read_raw(cht8305c_t *dev,
                      uint16_t   *raw_temp,
                      uint16_t   *raw_rh)
{

    int ret;
    
    /* 检查空指针 */
    if(!dev || !raw_temp || !raw_rh || !dev->i2c.inited)
    {
        #if CHT8305C_DEBUG_LOG 
        wm_log_error("CHT8305C read_raw: invalid param dev=%p, t=%p, rh=%p",
                     dev, raw_temp, raw_rh);
        #endif

        return WM_ERR_INVALID_PARAM;
    }

    uint8_t reg = CHT8305C_REG_DATA; /* 0x00 */
    uint8_t buf[4] = {0};

    /* 写寄存器地址 */
    ret  = bsp_i2c_write(&dev->i2c,&reg,1,NULL,0);

    if(ret != WM_ERR_SUCCESS)
    {
        #if CHT8305C_DEBUG_LOG 
        wm_log_error("CHT8305C write reg failed, ret=%d", ret);
        #endif
        return ret;
    }

    /* 手册规定必要的转换时间 */
    vTaskDelay(pdMS_TO_TICKS(CHT8305C_CONVERT_DELAY_MS));

    /* ret:获取bsp_i2c_read的读取状态     */
    /* 从bsp_i2c_read中读取四个字节放到buf */
    ret =  bsp_i2c_read(&dev->i2c,NULL,0,buf,sizeof(buf));

    if(ret != WM_ERR_SUCCESS)
    {
        #if CHT8305C_DEBUG_LOG 
        wm_log_error("CHT8305C read data failed, ret=%d", ret);
        #endif

        return ret;
    }
    /* 转换成16bit */
    *raw_temp = ((uint16_t)buf[0]<<8) | buf[1];
    *raw_rh   = ((uint16_t)buf[2]<<8) | buf[3];

    #if CHT8305C_DEBUG_LOG
    wm_log_info("CHT8305C raw temp=0x%04X, raw rh=0x%04X", *raw_temp, *raw_rh);
    #endif

    return WM_ERR_SUCCESS;
}                       

/* 温湿度转换 */
int cht8305c_read(cht8305c_t *dev,
                  float      *temp_c,
                  float      *rh)
{
    /* 指针检查 */
    if (!dev || !temp_c || !rh)
    {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("CHT8305C read: invalid param dev=%p, t=%p, rh=%p",
                     dev, temp_c, rh);
    #endif
        return WM_ERR_INVALID_PARAM;
    }

    uint16_t raw_t  = 0;
    uint16_t raw_rh = 0;

    /* 获取原始值 */
    int ret = cht8305c_read_raw(dev, &raw_t, &raw_rh);
    if (ret != WM_ERR_SUCCESS)
    {
    #if CHT8305C_DEBUG_LOG
        wm_log_error("CHT8305C read_raw failed, ret=%d", ret);
    #endif
        return ret;
    }

    /* 2. 按芯片公式把原始值转换成物理量 */
    float t = ((float)raw_t  * 165.0f) / 65535.0f - 40.0f;   // °C
    float h = ((float)raw_rh * 100.0f) / 65535.0f;           // %RH

    /* 把数据给外部变量 */
    *temp_c = t;
    *rh     = h;

    /* 把相对湿度钳在 0~100% 范围 */
    if (*rh < 0.0f)   *rh = 0.0f;
    if (*rh > 100.0f) *rh = 100.0f;

#if CHT8305C_DEBUG_LOG
    wm_log_info("CHT8305C T=%.2f C, RH=%.1f %% (raw_t=0x%04X, raw_rh=0x%04X)",
                t, h, raw_t, raw_rh);
#endif

    return WM_ERR_SUCCESS;
}


