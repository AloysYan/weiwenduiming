# 怎么点亮一颗LED
## 怎么点亮LED
- 灯两边的引脚存在电势差，PC13变成GND。

## 单片机介绍
- 晶振 8MHz
- stm32f103c8t6
- GPIO外设

## 外设操作
- 初始化——CUBEMX
- 启动——GPIO不需要，TIM用户启动
- 使用——调用HAL库函数

## CubeMX介绍
### 新建工程
1. 选择stm32f103c8t6。
2. 进行时钟配置和烧录配置：
   - 时钟：HSE（high speed extern，外部高速时钟）--> 时钟树配置 source-->HSE，总输入：72Mhz。
   - 烧录：SYS--> serial sire。
3. 外设初始化配置 PC13-->GPIO_OUTPUT。
4. 工程配置：名字，存储路径，IDE，库文件，.c/.h。
5. 生成代码。

## Keil使用
### 新建工程
- 关闭优化
- 配置下载器
### 代码历程
- int main()
  - 初始化部分 只执行一次
  - while(1) 循环执行
### 点灯
- HAL库函数手册 操作GPIO引脚的函数
### 调试功能
- Whach，步进，重启，全速运行