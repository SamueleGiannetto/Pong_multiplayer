/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_pong.h
** Last modified Date:  2022-6-1
** Last Version:        V1.00
** Descriptions:        High level pong management functions
** Correlated files:    lib_pong.c, funct_pong.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "pong.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "stdio.h"
#include "string.h"


/*----------------------------------------------------------------------------
  Funzioni che implementato il gioco del pong 
 *----------------------------------------------------------------------------*/


extern paddle paddlePos, rivalPaddlePos;
extern ball ballPos;
extern ballMovimento ballMov;
extern punteggio punt;
extern int pause;
int contattoPunteggio=0, contattoPunteggioRivale=0, movPaddleRivale, diffRivale, distanzabordoRivale, k;
char stringaPunteggio[10];  

/*funzione che sposta il paddle rivale di un pixel a destra o sinistra ogni volta che scatta il timer 0 'seguendo' il 
movimento della pallina*/

void movimentoPaddleRivale(void){
	
	if(pause != 1){					//se gioco non e' in pausa
		if(ballPos.centroX > rivalPaddlePos.centro  & rivalPaddlePos.estremoDx != 234){      //se pallina e' a destra rispetto al centro del paddle quest'ultimo la segue a meno che non sia già contro l'estremo destro
		{
			LCD_DrawLine(rivalPaddlePos.estremoSx,41,rivalPaddlePos.estremoSx,32,Black);
			LCD_DrawLine(rivalPaddlePos.estremoDx+1,41,rivalPaddlePos.estremoDx+1,32,Green);
		}
		rivalPaddlePos.estremoSx = rivalPaddlePos.estremoSx +1;
		rivalPaddlePos.estremoDx = rivalPaddlePos.estremoDx +1;
		rivalPaddlePos.centro = rivalPaddlePos.centro +1;
		}
		else if(ballPos.centroX < rivalPaddlePos.centro  & rivalPaddlePos.estremoSx != 5){	//se pallina e' a sinistra rispetto al centro del paddle quest'ultimo la segue a meno che non sia già contro l'estremo sinistro
		{
			LCD_DrawLine(rivalPaddlePos.estremoDx,41,rivalPaddlePos.estremoDx,32,Black);
			LCD_DrawLine(rivalPaddlePos.estremoSx+1,41,rivalPaddlePos.estremoSx+1,32,Green);
		}
		rivalPaddlePos.estremoSx = rivalPaddlePos.estremoSx -1;
		rivalPaddlePos.estremoDx = rivalPaddlePos.estremoDx -1;
		rivalPaddlePos.centro = rivalPaddlePos.centro -1;
		}
	}		
}


/*funzione che in base alle struct ballPos e ballMov sposta la pallina da una posizione con un certo movimento
ballPos contiene la posizione della pallina, ballMov dice di quanti pixel spostare pallina orizzontalmente e verticalmente e verso che direzione*/

void movimentoPallina(char nord_sud, char ovest_est){				
	static int i, j, n;

	if(nord_sud == 's' & ovest_est == 'o'){										//se spostamento verso sud-ovest
	
		for(i=0; i<ballMov.angolazioneX; i++){									//da 1 a 3 cicli in base ad angolazione, sposta da 1 a 3 volte pallina in orizzontale 
			LCD_DrawLine(ballPos.x+ballPos.dim-1-i, ballPos.y-ballPos.dim+1, ballPos.x+ballPos.dim-1-i, ballPos.y,Black);
			LCD_DrawLine(ballPos.x-1-i, ballPos.y-ballPos.dim+1, ballPos.x-1-i, ballPos.y,Green);
			if(ballPos.x-1-i == 5){																//se tocca il muro chiama la funzione colpitoMuro e poi return perchè non si muova più la pallina
					ballPos.x=ballPos.x-1-i;
					ballPos.y=ballPos.y;
					ballPos.centroX=ballPos.x+2;
					colpitoMuro();
					return;
			}
		}
				
		for(j=0; j<ballMov.angolazioneY; j++){									//da 1 a 3 cicli in base ad angolazione, sposta da 1 a 3 volte pallina in verticale
			LCD_DrawLine(ballPos.x-ballMov.angolazioneX, ballPos.y-ballPos.dim+1+j, ballPos.x+ballPos.dim-1-ballMov.angolazioneX, ballPos.y-ballPos.dim+1+j,Black);
			LCD_DrawLine(ballPos.x-ballMov.angolazioneX, ballPos.y+1+j, ballPos.x+ballPos.dim-1-ballMov.angolazioneX, ballPos.y+1+j,Green);
		  if(ballPos.y +j+1 == paddlePos.superficie-1){					//se la pallina ha colpito paddle giocatore uno
					ballPos.x=ballPos.x-ballMov.angolazioneX;
					ballPos.y=ballPos.y+j+1;
					ballPos.centroX=ballPos.x+2;
					colpitoPaddle(paddlePos,0);
					return;
			}
			if(ballPos.y +j+1 == 325){														//se la pallina e' caduta calcola il nuovo punteggio del gioatore 2
					punteggioNuovoRivale();
					return;
			}
		}
		ballPos.x=ballPos.x-i;																		//setto i nuovi valori sulla coordinata del vertice in basso a sinistra della pallina
		ballPos.y=ballPos.y+j;
		ballPos.centroX=ballPos.x+2;
	}
	
	
	if(nord_sud == 's' & ovest_est == 'e'){											//se spostamento verso sud-est
		
		for(i=0; i<ballMov.angolazioneX; i++){
			LCD_DrawLine(ballPos.x+i, ballPos.y-ballPos.dim+1, ballPos.x+i, ballPos.y,Black);
			LCD_DrawLine(ballPos.x+ballPos.dim+i, ballPos.y-ballPos.dim+1, ballPos.x+ballPos.dim+i, ballPos.y,Green);
			if(ballPos.x+ballPos.dim+i == 234){								//se la pallina ha colpito muro
					ballPos.x=ballPos.x+1+i;
					ballPos.y=ballPos.y;
					ballPos.centroX=ballPos.x+2;
					colpitoMuro();
					return;
			}
		}
				
		for(j=0; j<ballMov.angolazioneY; j++){
			LCD_DrawLine(ballPos.x+ballMov.angolazioneX, ballPos.y-ballPos.dim+1+j, ballPos.x+ballPos.dim-1+ballMov.angolazioneX, ballPos.y-ballPos.dim+1+j,Black);
			LCD_DrawLine(ballPos.x+ballMov.angolazioneX, ballPos.y+1+j, ballPos.x+ballPos.dim-1+ballMov.angolazioneX, ballPos.y+1+j,Green);
		  if(ballPos.y +j+1 == paddlePos.superficie-1){								//se la pallina ha colpito paddle giocatore uno
					ballPos.x=ballPos.x+ballMov.angolazioneX;
					ballPos.y=ballPos.y+j+1;
					ballPos.centroX=ballPos.x+2;
					colpitoPaddle(paddlePos,0);
					return;
			}
			if(ballPos.y +j+1 == 325){																	//se la pallina e' caduta oltre paddle giocatore uno
					punteggioNuovoRivale();										
					return;
			}
		}
		ballPos.x=ballPos.x+i;
		ballPos.y=ballPos.y+j;
		ballPos.centroX=ballPos.x+2;
	}
	
	
	
	if(nord_sud == 'n' & ovest_est == 'o'){																//se spostamento verso nord-ovest
		
		for(i=0; i<ballMov.angolazioneX; i++){
			LCD_DrawLine(ballPos.x+ballPos.dim-1-i, ballPos.y-ballPos.dim+1, ballPos.x+ballPos.dim-1-i, ballPos.y,Black);
			LCD_DrawLine(ballPos.x-1-i, ballPos.y-ballPos.dim+1, ballPos.x-1-i, ballPos.y,Green);
			if(ballPos.x-1-i==5){																	//se la pallina ha colpito muro
				ballPos.x=ballPos.x-1-i;
				ballPos.y=ballPos.y;
				ballPos.centroX=ballPos.x+2;
				colpitoMuro();
				return;
			}
		}
				
		for(j=0; j<ballMov.angolazioneY; j++){
			LCD_DrawLine(ballPos.x-ballMov.angolazioneX, ballPos.y-j, ballPos.x+ballPos.dim-1-ballMov.angolazioneX, ballPos.y-j,Black);
			LCD_DrawLine(ballPos.x-ballMov.angolazioneX, ballPos.y-ballPos.dim-j, ballPos.x+ballPos.dim-1-ballMov.angolazioneX, ballPos.y-ballPos.dim-j,Green);
			if(ballPos.y -ballPos.dim-j-1 == rivalPaddlePos.superficie){				//se la pallina ha toccato il paddle rivale
					ballPos.x=ballPos.x-ballMov.angolazioneX;
					ballPos.y=ballPos.y -j-1;
					ballPos.centroX=ballPos.x+2;
					colpitoPaddle(rivalPaddlePos,1);
					return;
			}
			if(ballPos.y -ballPos.dim-j-1 == 0){			//se la pallina e' caduta calcola il nuovo punteggio del gioatore 1 e cancella la pallina
					for(n=0; n<ballPos.dim; n++){
						LCD_DrawLine(ballPos.x-ballMov.angolazioneX+n,0,ballPos.x-ballMov.angolazioneX+n,5,Black);	
					}
					punteggioNuovo();
					return;
			}
		}
		ballPos.x=ballPos.x-i;
		ballPos.y=ballPos.y-j;
		ballPos.centroX=ballPos.x+2;
	}
	
	
	if(nord_sud == 'n' & ovest_est == 'e'){																	//se spostamento verso nord-est

		for(i=0; i<ballMov.angolazioneX; i++){
			LCD_DrawLine(ballPos.x+i, ballPos.y-ballPos.dim+1, ballPos.x+i, ballPos.y,Black);
			LCD_DrawLine(ballPos.x+ballPos.dim+i, ballPos.y-ballPos.dim+1, ballPos.x+ballPos.dim+i, ballPos.y,Green);
			if(ballPos.x+ballPos.dim+i==234){
				ballPos.x=ballPos.x+i+1;
				ballPos.y=ballPos.y;
				ballPos.centroX=ballPos.x+2;
				colpitoMuro();
				return;
			}
		}
				
		for(j=0; j<ballMov.angolazioneY; j++){
			LCD_DrawLine(ballPos.x+ballMov.angolazioneX, ballPos.y-j, ballPos.x+ballPos.dim-1+ballMov.angolazioneX, ballPos.y-j,Black);
			LCD_DrawLine(ballPos.x+ballMov.angolazioneX, ballPos.y-ballPos.dim-j, ballPos.x+ballPos.dim-1+ballMov.angolazioneX, ballPos.y-ballPos.dim-j,Green);
			if(ballPos.y -ballPos.dim-j-1 == rivalPaddlePos.superficie){   //se la pallina ha colpito paddle rivale
					ballPos.x=ballPos.x+ballMov.angolazioneX;
					ballPos.y=ballPos.y -j-1;
					ballPos.centroX=ballPos.x+2;
					colpitoPaddle(rivalPaddlePos,1);
					return;
			}
			if(ballPos.y -ballPos.dim-j-1 == 0){									//se la pallina e' caduta viene cancellata e calcolato il nuovo punteggio
					for(n=0; n<ballPos.dim; n++){
						LCD_DrawLine(ballPos.x+ballMov.angolazioneX+n,0,ballPos.x+ballMov.angolazioneX+n,5,Black);	
					}
					punteggioNuovo();
					return;
			}
		}
		ballPos.x=ballPos.x+i;
		ballPos.y=ballPos.y-j;
		ballPos.centroX=ballPos.x+2;
	}
	
	if((ballPos.x<25 & ballPos.y>155 & ballPos.y<175)){				//ristampa punteggio se pallina passandoci sopra l'ha cancellato
			contattoPunteggio=1;
	}else if(contattoPunteggio==1){
			sprintf(stringaPunteggio,"%d",punt.score);
			GUI_Text(10, 160, (uint8_t*) stringaPunteggio, White, Black);
			contattoPunteggio=0;
	}		
	
	if((ballPos.x>210 & ballPos.y>155 & ballPos.y<175)){		 //ristampa punteggio rivale se pallina passandoci sopra l'ha cancellato
			contattoPunteggioRivale=1;
	}else if(contattoPunteggioRivale==1){
			sprintf(stringaPunteggio,"%d",punt.scoreRivale);
			GUI_Text(220, 160, (uint8_t*) stringaPunteggio, White, Black);
			contattoPunteggioRivale=0;
	}		
}	


/*angolazioni possibili con spostamento di x pixel in orizzontale e y in verticale (x,y): (3,1),(2,1),(1,1),(1,2),(1,3) verso nord-est o verso nord-ovest
5 angolazioni differenti per ciascuna delle 2 direzioni in base a punto in cui il pixel centrale in basso della pallina tocca il paddle 
nella funzione il paddle e' diviso in 10 parti in modo da permette le 10 possibilità di spostamento*/

void colpitoPaddle(paddle paddlePosition, int rivale){
	if(ballPos.y == paddlePosition.superficie-1 | ballPos.y == paddlePosition.superficie+ballPos.dim){ 	
		if(ballPos.centroX >= paddlePosition.estremoSx & ballPos.centroX < paddlePosition.estremoSx+3){  //se prende estremo sinistro angolazione ampiamente verso sinistra
			if(rivale==0)													//a seconda di quale dei 2 paddle e' stato colpito la pallina si muoverà verso sud o verso nord
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='o';
			ballMov.angolazioneX=3;
			ballMov.angolazioneY=1;		
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+3 & ballPos.centroX < paddlePosition.estremoSx+8){ 
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='o';
			ballMov.angolazioneX=2;
			ballMov.angolazioneY=1;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+8 & ballPos.centroX < paddlePosition.estremoSx+13){  
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='o';
			ballMov.angolazioneX=1;
			ballMov.angolazioneY=1;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+13 & ballPos.centroX < paddlePosition.estremoSx+18){  
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='o';
			ballMov.angolazioneX=1;
			ballMov.angolazioneY=2;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+18 & ballPos.centroX < paddlePosition.estremoSx+20){  
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='o';
			ballMov.angolazioneX=1;
			ballMov.angolazioneY=3;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+20 & ballPos.centroX < paddlePosition.estremoSx+23){  
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='e';
			ballMov.angolazioneX=1;
			ballMov.angolazioneY=3;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+23 & ballPos.centroX < paddlePosition.estremoSx+28){  
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='e';
			ballMov.angolazioneX=1;
			ballMov.angolazioneY=2;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+28 & ballPos.centroX < paddlePosition.estremoSx+33){  
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='e';
			ballMov.angolazioneX=1;
			ballMov.angolazioneY=1;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+33 & ballPos.centroX < paddlePosition.estremoSx+38){  
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='e';
			ballMov.angolazioneX=2;
			ballMov.angolazioneY=1;
		}
		if(ballPos.centroX >= paddlePosition.estremoSx+38 & ballPos.centroX < paddlePosition.estremoSx+41){ 			//se prende estremo destro angolazione ampiamente verso destra
			if(rivale==0)
				ballMov.nord_sud='n';
			if(rivale==1)
				ballMov.nord_sud='s';
			ballMov.ovest_est='e';
			ballMov.angolazioneX=3;
			ballMov.angolazioneY=1;
		}
	}
}


/*calcola nuovo punteggio dopo contatto tra pallina e paddle e lo stampa*/
void punteggioNuovo(void){
	punt.score+=1;
	sprintf(stringaPunteggio,"%d",punt.score);
	GUI_Text(10, 160, (uint8_t*) stringaPunteggio, White, Black);
	if(punt.score==5){							//se primo giocatore ha vinto ferma paddle rivale e pallina
		fineGioco();
		disable_timer(0);		
		disable_timer(1);
		return;
	}
	startingBallConf();
}	


/*calcola nuovo punteggio dopo contatto tra pallina e paddle rivale e lo stampa*/
void punteggioNuovoRivale(void){
	punt.scoreRivale+=1;
	sprintf(stringaPunteggio,"%d",punt.scoreRivale);
	GUI_Text(220, 160, (uint8_t*) stringaPunteggio, White, Black);
	if(punt.scoreRivale==5){				//se rivale ha vinto chiama funzione per la fine del gioco
		fineGioco();
		disable_timer(0);		
		disable_timer(1);
		return;
	}
	startingBallConf();
}	


/*varia la direzione della pallina mantenendo l'angolo di incidenza*/
void colpitoMuro(void){
		
	if(ballPos.x == 5){ 							 //colpito muro di sinistra
		if(ballMov.nord_sud=='n'){
			ballMov.nord_sud='n';
			ballMov.ovest_est='e';				
		}
		else if(ballMov.nord_sud=='s'){
			ballMov.nord_sud='s';
			ballMov.ovest_est='e';			
		}
	}		
		
	if(ballPos.x+ballPos.dim-1 == 234){   //colpito muro di destra
		if(ballMov.nord_sud=='n'){
			ballMov.nord_sud='n';
			ballMov.ovest_est='o';
				
		}
		else if(ballMov.nord_sud=='s'){
			ballMov.nord_sud='s';
			ballMov.ovest_est='o';
		}
	}
}
	
	
/*controlla se il punteggio ottenuto nella partita e' un nuovo record, lo stampa, stampa piccolo tutorial e disabilita i timer per il movimento della pallina*/
void fineGioco(void){		
		if(punt.scoreRivale==5){																						//ha vinto rivale
			GUI_TextReverse(100, 90, (uint8_t*) "niw ouY", White, Black); 		//scritta You win speculare
			GUI_Text(90, 230, (uint8_t*) "You lose", White, Black);
		}
		if(punt.score==5){																									//ha vinto giocatore uno
			GUI_TextReverse(100, 90, (uint8_t*) "esol uoY", White, Black);		//scritta You lose speculare
			GUI_Text(90, 230, (uint8_t*) "You win", White, Black);
		}
		punt.score=0;
		punt.scoreRivale=0;
		GUI_Text(9, 295, (uint8_t*) "Press INT0 to reset the game", White, Black);

		return;		
}
