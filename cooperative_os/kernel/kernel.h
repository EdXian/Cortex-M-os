
#include <stdint.h>
#ifndef __KERNERL_H
#define __KERNERL_H

void os_taskstackinit(int i);
void oskernelcreatetask(void (*task0)(void),void (*task1)(void),void (*task2)(void));
void os_init();
void os_start(uint32_t data);
void os_taskyeild();
#endif