/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../led/led.h"
#include "../adc/adc.h"
#include "../pong/pong.h"
#include "../RIT/RIT.h"


extern int movimento;
extern ballMovimento ballMov;

uint16_t SinTable[20] =                                       // ÕýÏÒ±í                       
{
    410, 523, 627, 714, 778, 813, 789, 732, 
    650, 550, 438, 324, 217, 125, 55,  0  ,   
    41 , 105, 193, 353
};


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Ball speed 
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler (void)
{
	movimentoPaddleRivale();																				//funzione che sposta paddle rivale
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER1_IRQHandler (void)
{
	movimentoPallina(ballMov.nord_sud, ballMov.ovest_est);					//funzione che sposta la pallina
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/*
void TIMER2_IRQHandler (void)															//timer per il suono dal contatto pallina-paddle
{
	static int ticks=0;
	LPC_DAC->DACR = SinTable[ticks%20]<<6;
	ticks++;
	if(ticks==60) {
		ticks=0;
		disable_timer(2);
	}
	
  LPC_TIM2->IR = 1;			// clear interrupt flag 
  return;
}

void TIMER3_IRQHandler (void)															//timer per il suono dal contatto pallina-muro
{
	static int ticks2=0;
	LPC_DAC->DACR = SinTable[ticks2%20]<<6;
	ticks2++;
	if(ticks2==60) {
		ticks2=0;
		disable_timer(3);
	}
	
  LPC_TIM3->IR = 1;			// clear interrupt flag 
  return;
}
*/
/******************************************************************************
**                            End Of File
******************************************************************************/
