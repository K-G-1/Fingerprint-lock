# Fingerprint-lock
  Fingerprint lock，use fingerprint/ic card/key can unlock.

# 指纹密码锁
主控制器：stm32F4（主要是手上只有F4）

使用资源：

 1. as608指纹模块，串口驱动
 2. rc522 ic卡模块，spi驱动，s50卡
 3. oled IIC驱动
 3. 矩阵键盘
 4. 1.8寸tft彩屏，spi驱动
 5. 步进电机

ps：未使用USMART，前期调试使用oled，但是后面使用tft，所以有的函数不支持oled显示，待修改

