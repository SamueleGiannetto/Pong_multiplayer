/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "adc/adc.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "led/led.h"
#include "pong/pong.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

volatile paddle paddlePos, rivalPaddlePos;
volatile ball ballPos;
volatile ballMovimento ballMov;
volatile punteggio punt;

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  BUTTON_init();												/* BUTTON Initialization              */	
  LCD_Initialization();
	
	startingPitchConf();
	startingBallConf();
	startingPaddleConf();
	startingRivalPaddleConf();
	
	TP_Init();
	
	GUI_Text(9, 180, (uint8_t*) "Press KEY1 to start the game", White, Black);
	
	init_timer(0, 0x000072D0);							//inizializzo TIMER0 per il movimento del paddle
																				/* T = K / Fr = 0x001312D0/ 25MHz    */
																				/* T = K / Fr = 1.250.000 / 25MHz      */
																				/* T = 0.050s			*/	
	
	init_timer(1,0x000061A8);							/* TIMER1 Initialization              */  //TIMER1 per il movimento della palla
																				/* K = T*Fr = [s]*[Hz] = [s]*[1/s]	  */
																				/* T = K / Fr = 0x000061A8/ 25MHz    */
																				/* T = K / Fr = 25.000 / 25MHz      */
																				/* T = 0.001s			*/	
		
	//rimosso poichè la dimensione del codice eccedeva le dimensioni massime consentite da questa versione di keil
	//init_timer(2,444);  									//TIMER2 per il suono generato dal contatto con i paddle
	//init_timer(3,89);											//TIMER3 per il suono generato dal contatto con il muro
	
	init_RIT(0x004C4B40);									//RIT per il movimento del paddle
	enable_RIT();													//nella init e' settato a 100 MHz
	ADC_init();														//K = 50ms * 100 MHz = 5000000 ----> 0x4C4B40 
	
	
	LPC_SC->PCON |= 0x1;									 //power-down	mode										
	LPC_SC->PCON &= ~(0x2);						
	
	LPC_PINCON->PINSEL1 |= (1<<21);				//consente funzionamento dello speaker
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1)	
  {
		__ASM("wfi");
  }

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
