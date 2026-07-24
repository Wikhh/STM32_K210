# 通信与代码说明

## 1. K210 感知端

入口文件：`K210_STM32(angle).py`

### 1.1 初始化

- `fm.register()`：将物理 IO30、IO31 绑定到 UART1；
- `UART(..., 9600, 8, 0, 1)`：配置 9600、8N1；
- `lcd.init()`：初始化 LCD；
- `mic.init()`：初始化麦克风阵列；
- 四个 `KalmanFilter`：分别处理 X、Y、强度 R 和角度。

### 1.2 一维卡尔曼滤波

滤波器采用“预测—计算增益—校正”的标准形式：

```text
P_pred = P + Q
K      = P_pred / (P_pred + R)
x      = x + K × (z - x)
P      = (1 - K) × P_pred
```

其中：

- `Q` 表示过程噪声；
- `R` 表示测量噪声；
- `z` 是当前测量；
- `x` 是滤波后的估计。

当前参数为 `Q=0.01`、`R=0.1`，偏向平滑显示和控制输入。参数需要结合真实噪声和响应速度进一步整定。

### 1.3 声源角度计算

`mic.get_dir()` 返回 12 个方向通道的响应值。代码将每个通道视为间隔 30° 的方向向量：

```python
AngleX += b[i] * sin(i * π / 6)
AngleY += b[i] * cos(i * π / 6)
```

随后计算：

```text
强度 R = sqrt(X² + Y²)
角度 θ = atan(X / Y) + 象限补偿
```

只有通道响应 `b[i] >= 2` 时才参与合成。

### 1.4 发送策略

- `R > 15`：发送滤波后的声源角度；
- `R < 15`：发送 `0`，通知 STM32 停止；
- `R == 15`：当前代码不发送新帧。

发送周期约为 100 ms：

```python
packet_str = "@{:.2f}\r\n".format(angle)
uart_A.write(packet_str.encode())
```

## 2. 串口协议

协议为简单 ASCII 行协议：

```text
@<angle>\r\n
```

示例：

```text
@247.82\r\n
```

优点是便于串口工具直接观察；缺点是没有长度、校验和、序号和超时字段。

## 3. STM32 接收端

文件：`通信追踪2025.05.18/Project/uart2.c`

### 3.1 USART1 配置

- TX：PA9，复用推挽输出；
- RX：PA10，上拉输入；
- 波特率：9600；
- 数据格式：8N1；
- 接收方式：RXNE 中断；
- NVIC：抢占优先级 1，子优先级 1。

### 3.2 中断解析状态

`USART1_IRQHandler()` 按字符处理：

1. 收到 `@`：清空接收索引，开始新帧；
2. 收到普通字符：写入 `uart_rx_buffer`；
3. 收到 `\r` 或 `\n`：添加字符串结束符并置 `packet_ready=1`；
4. 缓冲区即将溢出：丢弃当前帧并将索引归零。

缓冲区长度为 64 字节，足够容纳当前角度帧。

### 3.3 主循环

文件：`通信追踪2025.05.18/User/main.c`

当 `packet_ready` 置位后：

```c
received_angle = atoi(uart_rx_buffer);
```

然后 OLED 显示：

- `angle`：接收角度；
- `speed`：速度变量；
- `data`：方向偏差控制量。

注意：`received_angle` 虽然声明为 `float`，但 `atoi()` 返回整数，所以小数部分会被丢弃。

## 4. STM32 执行端

### 4.1 TIM2 时基

文件：`通信追踪2025.05.18/Laasckp/timer.c`

系统时钟按工程配置为 72 MHz，TIM2：

- 预分频：72；
- 自动重装：10；
- 更新周期：10 μs。

中断内进一步生成：

- 1 ms 软件计数；
- 100 μs 电机软件 PWM 计数；
- 20 ms 舵机周期。

### 4.2 软件 PWM

`speed_count` 在 0～99 之间循环，`CarMove()` 比较它与左右电机占空比。因此 `left_speed_duty` 和 `right_speed_duty` 的绝对值可理解为 0～100 的占空比，符号决定方向。

### 4.3 声源控制

当前目标方向约为 250°：

```text
received_angle < 240° 或 > 265°：
    根据 250° - received_angle 的符号旋转

240° ≤ received_angle ≤ 265°：
    低速前进

received_angle == 0：
    停止
```

`motor.c` 中实现了增量式 `sound_PID()`，但当前控制路径未调用它。因此准确描述应是“区间判断 + 定速转向”，而不是 PID 声源闭环。

## 5. 关键全局变量

| 变量 | 定义位置 | 用途 |
|---|---|---|
| `uart_rx_buffer` | `uart2.c` | 保存角度字符串 |
| `packet_ready` | `uart2.c` | 完整帧到达标志 |
| `received_angle` | `uart2.c` | 主控制角度 |
| `left_speed_duty` | `motor.c` | 左轮方向与占空比 |
| `right_speed_duty` | `motor.c` | 右轮方向与占空比 |
| `sound_pwm` | `motor.c` | 目标角与实测角偏差 |
| `speed_count` | `motor.c` | 软件 PWM 计数 |

## 6. 推荐的代码改进顺序

1. 使用 `strtof()` 替代 `atoi()`；
2. 增加最近一次有效帧时间，通信超时后强制停车；
3. 将目标角度、容差、旋转速度、前进速度定义为具名宏；
4. 将较重的控制判断从 10 μs 中断移到固定周期任务；
5. 为帧增加校验和或 CRC；
6. 记录角度真值和测量值，再决定是否启用 PID。
