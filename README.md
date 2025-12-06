# A-Gateway-W800

基于 WinnerMicro W800 的可扩展智能家居网关项目。

本项目定位为“小型 IoT 网关内核”，后续可扩展：

- RS485（Modbus / 自定义协议）
- LVGL 图形界面（本地 HMI）
- 温湿度等环境传感器
- Telink BLE Mesh 灯光子系统（W800 作为 WiFi 网关）

当前版本重点完成 **RGB 指示灯驱动与网关应用架构的搭建**。

---

## 1. 工程结构

```text
A-Gateway-W800/
└─ Software/A-Gateway/
   ├─ main/
   │  ├─ src/                # 程序入口与公共头文件
   │  │  ├─ main.c
   │  │  └─ geteway_include.h
   │  ├─ app/                # 应用层（业务逻辑）
   │  │  └─ rgb_status.c/h   # 网关状态 → RGB 灯效
   │  ├─ bsp/                # 板级支持包（直接操作硬件）
   │  │  └─ bsp_rgb.c/h      # RGB GPIO / PWM 映射与初始化
   │  ├─ drivers/            # 通用驱动层（与具体板子解耦）
   │  │  └─ rgb_led.c/h      # RGB 颜色 / 亮度 / 呼吸等灯效算法
   │  ├─ tests/              # 各类测试单元
   │  │  ├─ test_rgb.c/h     # RGB 灯测试任务
   │  │  └─ test_config.h    # 测试功能编译开关
   │  └─ Middlewares/…       # SDK 中间件（来自 WM IoT SDK）
   └─ CMakeLists.txt         # 组件构建配置
