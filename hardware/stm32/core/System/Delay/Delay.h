#ifndef __DELAY_H
#define __DELAY_H

void SysTick_Configuration(uint32_t xms);
uint8_t SysTick_Timer_Inquire(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

#endif
