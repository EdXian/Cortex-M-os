
#include <stdint.h>
#ifndef __KERNERL_H
#define __KERNERL_H
struct tcb{
	int32_t *sp;	//the top of the stack
	
	struct tcb *next;
	
	int32_t delay;
	int32_t reload;
	int32_t period;
	
};
typedef struct tcb tcb_t;
void os_taskstackinit(int i);
void oskernelcreatetask(void (*task0)(void),void (*task1)(void),void (*task2)(void));
void os_init();
void os_start(uint32_t data);

void os_taskyeild();

#endif