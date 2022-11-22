/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           pong.h
** Last modified Date:  2021-12-20
** Last Version:        V1.00
** Descriptions:        Atomic pong init functions
** Correlated files:    lib_pong.c, funct_pong.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "pong.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"

/*----------------------------------------------------------------------------
  Functions that initialize pitch 
 *----------------------------------------------------------------------------*/

extern ball ballPos;
extern paddle paddlePos, rivalPaddlePos;
extern ballMovimento ballMov;
static int i;

void startingPitchConf(void){
	LCD_Clear(Black);
	GUI_Text(10, 160, (uint8_t*) "0", White, Black);		//punteggio
	GUI_Text(220, 160, (uint8_t*) "0", White, Black);		//punteggio
	
	for(i=0; i<5; i++){												//disegno i muri
		LCD_DrawLine(i,0,i,319,Red);
		LCD_DrawLine(239-i,0,239-i,319,Red);
	}
}


void startingBallConf(void){
	ballPos.dim=5;
	for(i=0; i<ballPos.dim; i++){
		//il centro della pallina si trova inizialmente al 160esimo pixel 
		LCD_DrawLine(234-i,156,234-i,160,Green);	
	}
	ballPos.x=230;
	ballPos.y=160;
	ballPos.centroX=232;
	ballMov.nord_sud='s';
	ballMov.ovest_est='o';
	ballMov.angolazioneX=1;				//angolazione iniziale di 45° a partire dalla metà del muro di destra
	ballMov.angolazioneY=1;
}


void startingPaddleConf(void){
	for(i=0; i<10; i++){					//disegno paddle 
		LCD_DrawLine(104, 287-i, 144, 287-i, Green);
	}		
	paddlePos.length=41;
	paddlePos.estremoSx=104;
	paddlePos.estremoDx=144;
	paddlePos.centro=(paddlePos.estremoSx+paddlePos.estremoDx)/2;
	paddlePos.superficie=278;					//altezza della superficie su cui palla dovrà rimbalzare
}

void startingRivalPaddleConf(void){
	for(i=0; i<10; i++){					//disegno paddle 
		LCD_DrawLine(104, 32+i, 144, 32+i, Green);
	}		
	rivalPaddlePos.length=41;
	rivalPaddlePos.estremoSx=104;
	rivalPaddlePos.estremoDx=144;
	rivalPaddlePos.centro=(paddlePos.estremoSx+paddlePos.estremoDx)/2;
	rivalPaddlePos.superficie=41;					//altezza della superficie su cui palla dovrà rimbalzare
}
