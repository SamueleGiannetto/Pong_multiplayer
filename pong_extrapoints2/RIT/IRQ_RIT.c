/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../adc/adc.h"
#include "../pong/pong.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int AD_current, key1=0, key2=0, int0=0, pause=2; // movimento; pause settata a 2 in modo che la palla inizi a muoversi solo dopo  
extern paddle paddlePos;
extern ball ballPos;
extern ballMovimento ballMov;
extern punteggio punt;
char stringaPunt[10];

void RIT_IRQHandler (void)
{					

	/* ADC management */
	if((LPC_GPIO1->FIOPIN & (1<<31)) == 0){								
		ADC_start_conversion();															//andrò a spostare il paddle
	}
	

	/* button management */
	if(key1>1){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			switch(key1){				
				case 2:				
				disable_RIT();	
				GUI_Text(9, 180, (uint8_t*) "Press KEY1 to start the game", Black, Black);  //per cancellare la scritta
				if(pause==2){
					pause=0;
					enable_timer(0);					//inizia a muoversi il paddle rivale
					enable_timer(1);					//inizia a muoversi la pallina
				}	
				enable_RIT();
					break;
				default:
					break;
			}
			key1++;
		}
		else {	/* button released */
			key1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	else{
			if(key1==1)
				key1++;
	}
	
	
		/* button management */
	if(key2>1){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			switch(key2){				
				case 2:				
				
				if(pause==0){					//se gioco in pausa
					pause=1;
					disable_timer(0);
					disable_timer(1);   //ferma movimento pallina		
				}
				else if(pause==1){		//se gioco riprende
					pause=0;
					enable_timer(0);
					enable_timer(1);		//riprende movimento pallina			
				}					
				
					break;
				default:
					break;
			}
			key2++;
		}
		else {	/* button released */
			key2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	else{
			if(key2==1)
				key2++;
	}
	
	
			/* button management */
	if(int0>1){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			switch(int0){				
				case 2:				
				disable_RIT();
				GUI_Text(9, 295, (uint8_t*) "Press INT0 to reset the game", Black, Black);	//per cancellare la scritta
				startingPitchConf();																												//campo da gioco e punteggi ripartono da punto d'inizio
				startingPaddleConf();
				startingRivalPaddleConf();
				startingBallConf();
				pause=2;																																			//la pallina inizierà a muoversi solo dopo aver schiacciato key1
				GUI_Text(9, 180, (uint8_t*) "Press KEY1 to start the game", White, Black);  
				punt.score=0;
				punt.scoreRivale=0;
				disable_timer(0);
				disable_timer(1);
				enable_RIT();
					break;
				default:
					break;
			}
			int0++;
		}
		else {	/* button released */
			int0=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	else{
			if(int0==1)
				int0++;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
