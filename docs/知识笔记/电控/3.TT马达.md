# <center>TT马达
### 驱动电机的核心原理：
直流电机的转动需要两个关键控制：
- 方向控制：通过改变电机两端的电压极性（正接 / 反接），控制电机正转 / 反转；
- 转速控制：通过 PWM 信号调节电机两端的平均电压，控制转速（占空比越高，转速越快）。

由于 STM32 的 GPIO 引脚输出电流极小（仅几 mA），无法直接驱动电机（需要几百 mA），因此必须通过电机驱动模块（如 L298N） 作为 “功率放大器”，接收单片机的弱电信号，输出强电驱动电机。

### 硬件准备：
|组件|作用|
|-|-|
|STM32F103C8T6|输出方向控制信号（GPIO）和 PWM 调速信号|
|L298N 驱动板|放大信号，控制电机正反转和转速|
|TT 直流电机|执行转动|
|锂电池（7.4V）|给电机和驱动板供电（电机需要大电流）|
|电源模块（3.3V）|给 STM32 供电（从锂电池取电降压）|

### 接线步骤（以 1 个电机为例，多电机同理）:
**1. 电机与 L298N 的连接**
电机的两根线（无正负之分）分别接 L298N 的OUT1和OUT2引脚（这两个引脚输出电机电源，极性决定转向）。
**2. L298N 与 STM32 的控制信号连接**
L298N 需要 3 个控制信号（2 个方向 + 1 个 PWM）：
L298N 引脚	功能	连接到 STM32 的引脚（示例）
IN1	控制电机正转信号	PA0（GPIO 输出）
IN2	控制电机反转信号	PA1（GPIO 输出）
ENA	控制转速的 PWM 信号	PA6（TIM3_CH1，PWM 输出）
**3. 电源连接（关键！）**
L298N 的12V 引脚 → 锂电池正极（给电机供电，7.4V 兼容）；
L298N 的GND 引脚 → 锂电池负极 + STM32 的 GND（共地，必须连接）；
电源模块输出3.3V → STM32 的 3.3V 引脚（给单片机供电）。

### CubeMX 软件配置：
**1. 配置 GPIO（方向控制）**
将 PA0、PA1 配置为 “GPIO_Output”（推挽输出，默认低电平）：
路径：Pinout → 点击 PA0/PA1 → 选择 “GPIO_Output”。
**2. 配置定时器（PWM 调速）**
选择 TIM3，配置通道 1 为 PWM 输出：
路径：Timers → TIM3 → Mode → “PWM Generation CH1”；
参数设置（PWM 频率 10kHz，适合电机）：
Prescaler（PSC）：71（72MHz ÷ (71+1) = 1MHz）；
Counter Period（ARR）：99（1MHz ÷ (99+1) = 10kHz）；
其他默认（自动重装载使能）。
**3. 生成代码**
点击 “Project Manager” 生成工程，确保MX_TIM3_Init()和MX_GPIO_Init()被正确初始化。

### 代码实现（驱动电机正转 / 反转 / 调速）
1. 电机控制函数（方向 + 转速）
在main.c中添加以下函数：
```
// 电机正转（参数：speed为0~100，代表占空比0%~100%）
void Motor_Forward(uint8_t speed) {
  // 方向控制：IN1=高电平，IN2=低电平
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);   // IN1=1
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // IN2=0
  
  // 调速：设置PWM占空比（speed=50 → 占空比50%）
  // 因ARR=99，所以比较值=speed（0~100对应0~99，近似100级调速）
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
}

// 电机反转
void Motor_Backward(uint8_t speed) {
  // 方向控制：IN1=低电平，IN2=高电平（与正转相反）
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); // IN1=0
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   // IN2=1
  
  // 同样通过PWM调速
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
}

// 电机停止
void Motor_Stop() {
  // 方向引脚都置低，电机无电流
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0); // 占空比0
}
```
1. 主函数测试
```
int main(void) {
  HAL_Init();
  SystemClock_Config();  // 系统时钟初始化（CubeMX生成）
  MX_GPIO_Init();        // GPIO初始化
  MX_TIM3_Init();        // 定时器3初始化（PWM）
  
  // 启动PWM输出（必须调用，否则无PWM信号）
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  while (1) {
    Motor_Forward(70);   // 正转（70%速度），持续2秒
    HAL_Delay(2000);
    
    Motor_Backward(50);  // 反转（50%速度），持续2秒
    HAL_Delay(2000);
    
    Motor_Stop();        // 停止1秒
    HAL_Delay(1000);
  }
}
```
### 多电机驱动（如 4 轮小车）
只需重复上述步骤，为每个电机分配独立的 GPIO（方向）和 PWM（调速）引脚：
前左电机：IN1=PA0，IN2=PA1，PWM=PA6；
前右电机：IN3=PA2，IN4=PA3，PWM=PA7；
后左电机：IN5=PB0，IN6=PB1，PWM=PB6；
后右电机：IN7=PB2，IN8=PB3，PWM=PB7；
分别编写 4 个电机的控制函数，再通过组合控制实现小车的前进、横移等动作（如之前的麦克纳姆轮代码）。

### 常见问题解决
**电机不转：**
检查 L298N 的 12V 电源是否接锂电池（必须通电）；
确认 GND 共地（STM32、L298N、锂电池负极连通）；
用万用表测 ENA 引脚是否有 PWM 电压（0~ 电源电压波动）。
**方向反了：**
交换电机接 L298N 的 OUT1 和 OUT2 引脚；
或在代码中交换 IN1 和 IN2 的高低电平。
**转速不均匀：**
不同电机存在个体差异，可微调 PWM 占空比（如 MotorA 用 70，MotorB 用 75）；
确保电源电压稳定（锂电池电量充足）。

### 总结：
**驱动电机的核心是 “方向由 GPIO 电平控制，转速由 PWM 占空比控制”，通过 L298N 放大功率后驱动。**