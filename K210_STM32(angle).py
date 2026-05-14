import math
import time
from Maix import MIC_ARRAY as mic
import lcd
from machine import UART
from fpioa_manager import fm
import struct
# 初始化串口

fm.register(30, fm.fpioa.UART1_TX, force=True)
fm.register(31, fm.fpioa.UART1_RX, force=True)
uart_A = UART(UART.UART1, 9600, 8, 0, 1)
# 卡尔曼滤波器类
class KalmanFilter:
    def __init__(self, Q, R):
        self.Q = Q
        self.R = R
        self.x = 0.0
        self.P = 1.0

    def filter(self, z):
        x_pred = self.x
        P_pred = self.P + self.Q
        K = P_pred / (P_pred + self.R)
        x_upd = x_pred + K * (z - x_pred)
        P_upd = (1 - K) * P_pred
        self.x = x_upd
        self.P = P_upd
        return self.x

# 初始化模块
lcd.init()
mic.init()

# 初始化卡尔曼滤波器
kf_x = KalmanFilter(Q=0.01, R=0.1)
kf_y = KalmanFilter(Q=0.01, R=0.1)
kf_r = KalmanFilter(Q=0.01, R=0.1)
kf_angle = KalmanFilter(Q=0.01, R=0.1)



def get_mic_dir(kf_x, kf_y, kf_r, kf_angle):
    AngleX = 0
    AngleY = 0
    AngleAddPi = 0
    mic_list = []

    # 获取原始的声源黑白位图，尺寸 16*16
    imga = mic.get_map()

    # 获取声源方向并设置LED显示
    b = mic.get_dir(imga)
    a = mic.set_led(b, (0, 0, 255))

    # 计算坐标和角度
    AngleX, AngleY = 0.0, 0.0
    for i in range(len(b)):
        if b[i] >= 2:
            AngleX += b[i] * math.sin(i * math.pi / 6)
            AngleY += b[i] * math.cos(i * math.pi / 6)
    AngleX = round(AngleX, 6)  # 计算坐标转换值
    AngleY = round(AngleY, 6)

    if AngleY < 0:
        AngleAddPi = 180
    if AngleX < 0 and AngleY > 0:
        AngleAddPi = 360

    if AngleX != 0 or AngleY != 0:  # 参数修正
        if AngleY == 0:
            Angle = 90 if AngleX > 0 else 270  # 填补X轴角度
        else:
            Angle = AngleAddPi + round(math.degrees(math.atan(AngleX / AngleY)), 4)  # 计算角度
        AngleR = round(math.sqrt(AngleY * AngleY + AngleX * AngleX), 4)  # 计算强度

        # 应用卡尔曼滤波
        AngleX = kf_x.filter(AngleX)
        AngleY = kf_y.filter(AngleY)
        AngleR = kf_r.filter(AngleR)
        Angle = kf_angle.filter(Angle)

        mic_list.append(AngleX)
        mic_list.append(AngleY)
        mic_list.append(AngleR)
        mic_list.append(Angle)

    imgb = imga.resize(160, 160)
    imgc = imgb.to_rainbow(1)

    lcd.display(imgc)

    return mic_list

#这里数据包格式没有用到
def pack_float_coords(x, y):
    packet = bytearray()
    packet.append(0xFF)                      # 帧头
    packet.extend(struct.pack('f', x))       # 4字节 float x
    packet.extend(struct.pack('f', y))       # 4字节 float y
    packet.append(0xFE)                      # 帧尾
    return packet


while True:
    result = get_mic_dir(kf_x, kf_y, kf_r, kf_angle)
    if result:
        # 获取 X, Y 坐标
        x = result[0]
        y = result[1]
        r = result[2]
        angle = result[3]

        # 如果强度大于11，发送角度
        if r > 15:
            packet_str = "@{:.2f}\r\n".format(angle)  # 只发送角度
            uart_A.write(packet_str.encode())
            print("发送角度到STM32：", packet_str)
        if r <15:
            packet_str = "@0\r\n".format(angle)  # 发送0,让电机停止
            uart_A.write(packet_str.encode())
            print("发送角度到STM32：", packet_str)

    time.sleep(0.1)

mic.deinit()
lcd.deinit()
uart.deinit()
