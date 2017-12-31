/*************************
 * stm32 uart main.c
 *************************/
 
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
	GPIO_InitTypeDef GPIO_InitStruct;
/* User defined function prototypes */
void GPIOB_Init(void);
void USART1_Init(void);
void led_toggle(void);
void Button_Init(void); 
int InitCompleted = 0;
 void Delay(__IO uint32_t ms)
 {
  long i;
	i = ms*10100;
	while(i)
	{
		i--;
   }
 } 
 
int main(void)
{
		Delay(5000);
	  Button_Init();
    GPIOB_Init();
    /* Initialize USART1 */
    USART1_Init();

    while(1)
    {
			if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_1))
			{
				while (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_1));
				// Turn on LED on PA0 (LED circuit is active low)
				if (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_8)) {
					GPIO_SetBits(GPIOB, GPIO_Pin_8);
				} else {
					GPIO_ResetBits(GPIOB, GPIO_Pin_8);
				}
				
			}
			
			if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_0))
			{
				while (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_0));
				if (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_9)) {
					GPIO_SetBits(GPIOB, GPIO_Pin_9);
				} else {
					GPIO_ResetBits(GPIOB, GPIO_Pin_9);
				}
			}	
    }
}   

void Button_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// Cofigure PA0 as open-drain output
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Cofigure PA1 as input with internal pull-up resistor
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
/***********************************************
 * Initialize GPIOA PIN8 as push-pull output
 ***********************************************/
void GPIOB_Init(void)
{
    /* Bit configuration structure for GPIOA PIN8 */
    GPIO_InitTypeDef gpioa_init_struct = { GPIO_Pin_8 |GPIO_Pin_9, GPIO_Speed_50MHz, 
                                           GPIO_Mode_Out_PP };
                                             
    /* Enable PORT A clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    /* Initialize GPIOA: 50MHz, PIN8, Push-pull Output */
    GPIO_Init(GPIOB, &gpioa_init_struct);   
     
    /* Turn off LED to start with */
    GPIO_ResetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9);
}
 
/*****************************************************
 * Initialize USART1: enable interrupt on reception
 * of a character
 *****************************************************/
void USART1_Init(void)
{
    /* USART configuration structure for USART1 */
    USART_InitTypeDef usart1_init_struct;
    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpioa_init_struct;
     
    /* Enalbe clock for USART1, AFIO and GPIOA */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | 
                           RCC_APB2Periph_GPIOA, ENABLE);
                            
    /* GPIOA PIN9 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_9;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpioa_init_struct);
    /* GPIOA PIN9 alternative function Rx */
    gpioa_init_struct.GPIO_Pin = GPIO_Pin_10;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioa_init_struct);
 
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);  
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart1_init_struct.USART_BaudRate = 9600;   
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;  
    usart1_init_struct.USART_StopBits = USART_StopBits_1;   
    usart1_init_struct.USART_Parity = USART_Parity_No ;
    usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
}
 
/*******************************************
 * Toggle LED 
 *******************************************/

 
/**********************************************************
 * USART1 interrupt request handler: on reception of a 
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
			if((char)USART_ReceiveData(USART1) == 's') {
				InitCompleted = 1;
			}
			if(InitCompleted == 1) {
			switch((char)USART_ReceiveData(USART1))
			{
				case 'a':
				{
				 	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)0);								
					break;
				}
					case 'b':
				{
					GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)1);			
					break;
				}
				case 'c':
				{
					GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)0);
					break;
				}
				case 'd':
				{
					GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)1);
					break;
				}
			}
		}
        
    } 
     
    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}   
