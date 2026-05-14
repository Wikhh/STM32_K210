#ifndef __uart2_H__
#define __uart2_H__

#include <stdio.h>

extern char Serial_RxPacket[];
extern char uart_rx_buffer[];
extern uint8_t Serial_RxFlag;
extern volatile uint8_t packet_ready;
extern float received_angle;
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);


#endif
