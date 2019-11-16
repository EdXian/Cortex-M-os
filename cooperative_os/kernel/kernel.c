#include "kernel.h"
#include "stdio.h"
#include "stdint.h"
#include "stm32f4xx.h"
#define task_num 3
#define stacksize 1024  //1 word = 4bytes 

#define FREQ 16000000
#define SYSPRI3    (* ((volatile uint32_t *)0xE000ED20))
	#define INTCTRL  (* ((volatile uint32_t *)0xE000ED04))
uint32_t prescaler;
uint32_t *system_tick;
//os functioin is prefixed with os
void osscehdulerlaunch();


struct tcb{
	int32_t *sp;	//the top of the stack
	
	struct tcb *next;
	
};

typedef struct tcb tcb_t;

tcb_t tcbs[task_num];
tcb_t *current_tcb;

int32_t TCB_STACK[task_num][stacksize];

void os_taskstackinit(int i){
//init the stack of the task
tcbs[i].sp = &TCB_STACK[i][stacksize-16];
TCB_STACK[i][stacksize-1]=0x1000000;	//xpsr
	//TCB_STACK[i][stacksize-2] should be the address of the pc 
	//for debug  (save automatically)
	TCB_STACK[i][stacksize-3] = 0x14141414;  //R14
	TCB_STACK[i][stacksize-4] = 0x12121212;  //R12
	TCB_STACK[i][stacksize-5] = 0x03030303;  //R3
	TCB_STACK[i][stacksize-6] = 0x02020202;  //R2
	TCB_STACK[i][stacksize-7] = 0x01010101;  //R1
	TCB_STACK[i][stacksize-8] = 0x00000000;  //
	
	//save manually
	TCB_STACK[i][stacksize-9] = 0x11111111;	//r11
	TCB_STACK[i][stacksize-10] = 0x11111111;//r10
	TCB_STACK[i][stacksize-11] = 0x11111111;//9
	TCB_STACK[i][stacksize-12] = 0x11111111;//8
	TCB_STACK[i][stacksize-13] = 0x11111111;//7
	TCB_STACK[i][stacksize-14] = 0x11111111;//6
	TCB_STACK[i][stacksize-15] = 0x11111111;//5
	TCB_STACK[i][stacksize-15] = 0x11111111;//5

	
}

void oskernelcreatetask(void (*task0)(void),void (*task1)(void),void (*task2)(void)){

	__disable_irq(); //mask to disable  IRQ
	
	tcbs[0].next = &tcbs[1];
	tcbs[1].next = &tcbs[2];
	tcbs[2].next = &tcbs[0];
	
	//store  the pc into the stack
	os_taskstackinit(0);
	TCB_STACK[0][stacksize-2] =   (int32_t)(task0);         //program counter

		os_taskstackinit(1);
	TCB_STACK[1][stacksize-2] =   (int32_t)(task1);   
	
		os_taskstackinit(2);
	TCB_STACK[2][stacksize-2] =   (int32_t)(task2);   
	
	current_tcb = &tcbs[0];
	
	
	__enable_irq();
	
	
}

void os_init(){
		prescaler = (FREQ/1000);

}

void os_taskyeild(){
	//forcing  to do contextswitch 
	
	SysTick->VAL=0;
	INTCTRL=0x04000000;

}


void os_start(uint32_t data){
	SysTick->CTRL= 0;	//setbuf register
	SysTick->VAL=0;
	SysTick->LOAD=(data* prescaler)-1;  //counting begin from 0
	
	SYSPRI3 = (SYSPRI3&0x00FFFFFF)|0xE000000;	//priority7

	SysTick->CTRL = 0x00000007;
	osscehdulerlaunch();
}
