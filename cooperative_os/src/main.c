#include "stdio.h"
#include <kernel.h>
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "Board_LED.h"                  // ::Board Support:LED

UART_HandleTypeDef usartHandle;


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
 /* Place your implementation of fputc here */
 /* e.g. write a character to the USART3 and Loop until the end of transmission */
 HAL_UART_Transmit(&usartHandle, (uint8_t *)&ch, 1, 0xFFFF);
 
 return ch;
}

#define quanta 10
int32_t count0,count1,count2;
char flag =0;
	GPIO_InitTypeDef  led_port;

float a=5.0;
uint8_t	s[10];
//PA2 PA3
void task0(void){

	while(1){
	//	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
		
		__disable_irq();
		
		
		 HAL_UART_Transmit(&usartHandle, "task0\n",15, 0xFFFF);
		__enable_irq();
		count0++;
		
		
		os_taskyeild();
	}
}

void task1(void){

	while(1){
		
				//HAL_UART_Transmit(&usartHandle,(uint8_t*)"task2\n",6,0xffff);
	__disable_irq();
		
		
		 HAL_UART_Transmit(&usartHandle, "task1\n",15, 0xFFFF);
		__enable_irq();
			count1++;
		os_taskyeild();


	}

}

void task2(void){
	while(1){
//				__disable_irq();
//				HAL_UART_Transmit(&usartHandle,(uint8_t*)"task3\n",6,0xffff);
//								__disable_irq();
		__disable_irq();
		 HAL_UART_Transmit(&usartHandle, "task2\n",15, 0xFFFF);
		__enable_irq();
				count2++;
		os_taskyeild();

	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
		
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
}
void initGPIOs(void) {

	GPIO_InitTypeDef uart_port;
	__HAL_RCC_GPIOA_CLK_ENABLE();  
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	uart_port.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	
	uart_port.Pin 		= GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5;
	uart_port.Mode 	= GPIO_MODE_AF_PP;
	uart_port.Pull 	= GPIO_PULLUP;
	uart_port.Alternate = GPIO_AF7_USART2;
	
	HAL_GPIO_Init(GPIOA, &uart_port);
	
	
	
	
		led_port.Pin 		= GPIO_PIN_13;
	led_port.Mode 	= GPIO_MODE_AF_PP;
	led_port.Pull 	= GPIO_PULLUP;

		HAL_GPIO_Init(GPIOB, &led_port);

  
}





void initUSART(void) {

	__HAL_RCC_USART2_CLK_ENABLE();
  

  usartHandle.Instance = USART2;
  usartHandle.Init.BaudRate = 9600;
  usartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  usartHandle.Init.StopBits = UART_STOPBITS_1;
  usartHandle.Init.Parity = UART_PARITY_NONE;
  usartHandle.Init.Mode = UART_MODE_TX_RX;
  usartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  usartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&usartHandle) != HAL_OK){
 
   			//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
			flag =1;
 
	}

}









int main(void){

	LED_Initialize();
	LED_On(2);
	//HAL_UART_MspInit(&usartHandle);
	initUSART();
	os_init();
	oskernelcreatetask(&task0,&task1,&task2);
	os_start(100);
	
}


