#include "geteway_include.h"

#undef  LOG_TAG
#define LOG_TAG "bsp_i2c"

int bsp_i2c_bus_init(bsp_i2c_bus_t *bus,
                     const char    *controller_name,
                     uint16_t       addr_7bit,
                     uint32_t       speed_hz)
{
    if (!bus || !controller_name) {
        return WM_ERR_INVALID_PARAM;
    }

    // 调 OPS 驱动层，拿到底层 device 实例（可能是硬件 I2C，也可能是软 I2C）
    wm_device_t *dev = wm_drv_i2c_init(controller_name);
    if (dev == NULL) {
        wm_log_error("i2c controller %s init failed", controller_name);
        return WM_ERR_FAILED;
    }

    memset(bus, 0, sizeof(*bus));
    bus->dev         = dev;
    bus->cfg.addr    = addr_7bit;
    bus->cfg.speed_hz = speed_hz;
    bus->inited      = true;

    return WM_ERR_SUCCESS;
}

void bsp_i2c_bus_deinit(bsp_i2c_bus_t *bus)
{
    if (!bus || !bus->inited || !bus->dev) {
        return;
    }

    wm_drv_i2c_deinit(bus->dev);

    bus->dev    = NULL;
    bus->inited = false;
}

/**
 * @brief 底层公共封装：内部组 wm_drv_i2c_config_t，然后直接调用 SDK
 */
static int bsp_i2c_transfer_read(bsp_i2c_bus_t *bus,
                                 const void    *sub_addr,
                                 uint32_t       sub_addr_size,
                                 void          *buf,
                                 uint32_t       size)
{
    wm_drv_i2c_config_t cfg;

    cfg.addr     = bus->cfg.addr;
    cfg.speed_hz = bus->cfg.speed_hz;

    return wm_drv_i2c_read(bus->dev, &cfg,
                           sub_addr, sub_addr_size,
                           buf, size);
}

static int bsp_i2c_transfer_write(bsp_i2c_bus_t *bus,
                                  const void    *sub_addr,
                                  uint32_t       sub_addr_size,
                                  const void    *data,
                                  uint32_t       size)
{
    wm_drv_i2c_config_t cfg;

    cfg.addr     = bus->cfg.addr;
    cfg.speed_hz = bus->cfg.speed_hz;

    return wm_drv_i2c_write(bus->dev, &cfg,
                            sub_addr, sub_addr_size,
                            data, size);
}

int bsp_i2c_read(bsp_i2c_bus_t *bus,
                 const void    *sub_addr,
                 uint32_t       sub_addr_size,
                 void          *buf,
                 uint32_t       size)
{
    if (!bus || !bus->inited || !bus->dev || !buf || size == 0) {
        return WM_ERR_INVALID_PARAM;
    }

    int ret = bsp_i2c_transfer_read(bus, sub_addr, sub_addr_size, buf, size);
#ifdef CONFIG_BSP_I2C_DEBUG
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("i2c read failed, addr=0x%02X, ret=%d", bus->cfg.addr, ret);
    }
#endif
    return ret;
}

int bsp_i2c_write(bsp_i2c_bus_t *bus,
                  const void    *sub_addr,
                  uint32_t       sub_addr_size,
                  const void    *data,
                  uint32_t       size)
{
    if (!bus || !bus->inited || !bus->dev) {
        return WM_ERR_INVALID_PARAM;
    }

    int ret = bsp_i2c_transfer_write(bus, sub_addr, sub_addr_size, data, size);
#ifdef CONFIG_BSP_I2C_DEBUG
    if (ret != WM_ERR_SUCCESS) {
        wm_log_error("i2c write failed, addr=0x%02X, ret=%d", bus->cfg.addr, ret);
    }
#endif
    return ret;
}
