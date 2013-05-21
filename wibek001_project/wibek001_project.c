/*
 * wibek001_project.c
 *
 * Created: 5/18/2013 11:08:24 AM
 *  Author: willibeamin
 */ 

#include <avr/io.h>
#include <bit.h>
#include <timer.h>
#include <scheduler.h>
#include <keypad.h>
#include <tone.h>
#include <lcd_8bit_task.h>
#include <equation.h>


//*************HAND SHAKING WITH ANSWER AND SCROLLING EQUATIONS
unsigned char sendAnswer = 0; 
unsigned char checkAnswer = 0; 

void init_PWM() {
	TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS22);
}

void set_PWM(double frequency) {
	unsigned long temp;
	if (frequency < 1)
	OCR2 = 0;
	else
	OCR2 = (int)(16000000 / (128 * frequency)) - 1;
}

enum SpeakMel{  SMON, SMOFF } speakMel;
unsigned char melody[165] = { 4, 0, 0, 4, 0, 0, 6, 6, 7, 7, 4, 0, 0, 4, 0, 0, 3, 3, 2, 2, 4, 0, 0, 4, 0, 0, 6, 6, 7, 7, 4, 0, 0, 4, 0, 0, 3, 3, 2, 2, 4, 0, 0, 4, 0, 0, 6, 6, 7, 7, 4, 0, 0, 4, 0, 0, 3, 3, 2, 2, 4, 0, 0, 4, 0, 0, 6, 6, 7, 7, 4, 0, 0, 4, 0, 0, 3, 3, 2, 2,
							  6, 4, 1, 1, 1, 1, 1, 1, 1, 1, 0, 6, 4, 9, 9, 9, 9, 9, 9, 9, 9, 0, 6, 4, 8, 8, 8, 8, 8, 8, 8, 8, 0, 10, 8, 0, 0, 0, 0, 0, 0, 0, 0,    
						     10, 12, 2, 2, 2, 2, 2, 2, 2, 2, 0,  10, 12, 3, 3, 3, 3, 3, 3, 3, 3, 0, 10, 12, 11, 11, 11, 11, 11, 11, 11, 11, 0, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0 }; // };
	
unsigned char smcounter = 0; 
void speakerMelody()
{
	switch( speakMel )
	{
		case -1: 
			speakMel = SMON;
			break; 
		case SMON:
			speakMel = SMOFF; 
			break; 
		case SMOFF: 
			speakMel = SMON; 
			break;
		default: 
			speakMel = SMON;
			break;  
	}
	
	switch( speakMel )
	{
		case -1:
			break;
		case SMON:
			if(smcounter == 165 )
			{
				smcounter = 0;
			}
			set_PWM( tone( melody[smcounter++] ));
			break;
		case SMOFF:
			break;
		default:
			break;
	}
	
}
unsigned char pvalue = 0;
unsigned char inputTemp;
enum LCDScroll { CLEAR, SHIFT, PRINT} lcdscroll;
E equations; 
unsigned char scrollCount = 0; 
unsigned char counter = 0; 
unsigned char answer = 4; 
unsigned char correctAnswer = 0;  
unsigned char tmpA = 0; 
void scroller()
{
	
	switch(lcdscroll)
	{
		case -1:
			lcdscroll = CLEAR; 
			break;
		case CLEAR: 
			lcdscroll = SHIFT;
			break; 
		case SHIFT: 
			lcdscroll = PRINT;
			break; 
		case PRINT: 
			lcdscroll = CLEAR; 
			break; 
		default: 
			lcdscroll = -1; 
			break; 
	}
	
	switch(lcdscroll)
	{
		case -1:
			break;
		case CLEAR:
			LCD_go_g = 0; 
			break;
		case SHIFT:
			if (scrollCount > 10)
			{
				LCD_char_pos = 0;
				if( LCD_string_g[0] == '=' || inputTemp == 0 )
				{
					
					correctAnswer++; 
					if( correctAnswer > 2 )
					{
						tmpA = SetBit( tmpA, 0, 1 ); 
					} 
					if( correctAnswer > 4 )
					{
						tmpA = SetBit( tmpA, 1, 1 );
					}
					if( correctAnswer > 6 )
					{
						tmpA = SetBit( tmpA, 2, 1 );
					}
					if( correctAnswer > 8 )
					{
						tmpA = SetBit( tmpA, 3, 1 );
					}
					if ( correctAnswer == 0 ) 
					{
						tmpA = 0; 	
					}
					unsigned char left = rand() % 10;
					unsigned char right = rand() % 10;
					answer = left + right; 
					char  temp[32];
					strcpy(LCD_string_g, "               ");
					strcat( LCD_string_g, convertFromChar2String(left));
					strcat( LCD_string_g, " + ");
					strcat( LCD_string_g, convertFromChar2String(right));
					strcat( LCD_string_g, "  =                                                  ");
					//strcpy( LCD_string_g, temp) ; 
				}					
				strcpy( LCD_string_g,  strcat( LCD_string_g, "                                                  ") + 1 );
				counter++; 
				scrollCount = 0; 
			}	 
			scrollCount++;
			break;
		case PRINT:
			LCD_go_g = 1; 
			break;
		default:
			break;
	}
	PORTA = tmpA; 
}

enum States { UNHOLD, CAPTURE, HOLD } state; 
unsigned char tp = 0; 
unsigned char t[10] = {"                                "};

unsigned char* ansArray;
unsigned char answerCounter = 0;
void keypadtest() 
{
	pvalue = GetKeypadKey(); 
	switch ( state )
	{
		case -1: 
			state = UNHOLD;
			break; 
		case UNHOLD: 
			if( pvalue != '\0' )
			{ 
				state = CAPTURE;
			}
			else
			{
				state = UNHOLD;
			}
			break;
		case CAPTURE: 
			state = HOLD; 
			break; 
		case HOLD: 
			 if( pvalue == '\0' )
			 {
				 state = UNHOLD;
			 }
			 else
			 {
				 state = HOLD;
			 }
			break; 
		default: 
			state = UNHOLD;
			break; 
	}
	
	switch ( state )
	{
		case -1: 
			break; 
		case UNHOLD: 
			break; 
		case CAPTURE:
			if( pvalue == '*' )
			{
				t[answerCounter++] = '\0';
				answerCounter = 0;
				inputTemp = answer - atoi(t); 
			}	
			else
			{
				t[answerCounter++] = pvalue;
			}			
			//strcpy( LCD_string_g, t);
			break; 
		case HOLD: 
			LCD_go_g = 1; 
			break; 
		default: 
			break; 
	}
}

enum Ans { TYPEIN, CONVERT } ans;
unsigned char inputTemp; 
unsigned char* ansArray;  

void equationAnswer() 
{
	inputTemp =  GetKeypadKey();  
	switch( ans ) 
	{
		case -1: 
			//strcpy( ansArray, "");
			break; 
		case TYPEIN: 
			if( answerCounter > 9  ||  inputTemp == '\0' )
			{
				ans = CONVERT;
			}
			else 
			{
				ans = TYPEIN;
			}
			break; 
		case CONVERT: 
			ans = TYPEIN; 
			break; 
		default: 
			ans = -1; 
			break; 
	}
	switch( ans )
	{
		case -1:
			break;
		case TYPEIN:
			if ( inputTemp != '\0' && inputTemp != '*' && inputTemp != '#' )
			{
				answerCounter++;
				strcat(ansArray, inputTemp );
			}			
			break;
		case CONVERT:
			answer = 0; //answer - atoi( ansArray ); 
			strcpy(ansArray, ""); 
			sendAnswer = 1; 
			counter = 0; 
			break;
		default:
			break;
	}
}
enum powerUpClear { pcUP, pcDOWN} pc;
unsigned char tmpA2 = 0; 
void powerUP() 
{
	tmpA2 = PINA; 
	switch( pc ) 
	{
		case -1: 
			break; 
		case pcUP: 
			if( GetBit( ~tmpA2, 4 ) )
			{
				correctAnswer = 0; 
				pc = pcDOWN;  
			} 
			else
			{
				pc = pcUP; 
			}
			break; 
		case pcDOWN:
			if( !GetBit( ~tmpA2, 4 ) )
			{
				pc = pcUP;
			}
			else
			{
				pc = pcDOWN;
			}
			break;  
		default: 
			break; 
	}
	
	switch( pc )
	{
		case -1:
			break;
		case pcUP:
			answerCounter = 0; 
			break;
		case pcDOWN:
			break;
		default:
			break;
	}
	
	
}


int main(void)
{
	init_PWM();
	//while( GetKeypadKey() == '\0' ); 
	//equations = load();
	strcpy( LCD_string_g,  "               2 + 2 =                                           " );
	//strcpy( LCD_string_g, equations[1].e ) ; 
	//LCD_write_str = 0;
	DDRB = 0xFF; // Set port B to output
	DDRC = 0xFF; // Set port D to output
	DDRA = 0xF7; PORTA = 0x08;
	DDRD = 0xF0; PORTD = 0x0F;

	// Period for the tasks
	unsigned long int SMTick1_calc = 100;
	unsigned long int SMTick2_calc = 1;
	unsigned long int SMTick3_calc = 5;
	unsigned long int SMTick4_calc = 20;
	unsigned long int SMTick5_calc = 10;
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);
	tmpGCD = findGCD(tmpGCD, SMTick4_calc);
	tmpGCD = findGCD(tmpGCD, SMTick5_calc);
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;
	unsigned long int SMTick4_period = SMTick4_calc/GCD;
	unsigned long int SMTick5_period = SMTick5_calc/GCD;
	//Declare an array of tasks
	static task task1, task2, task3, task4, task5;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &speakerMelody;//Function pointer for the tick.

	// Task 2
	task2.state = -1;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &LCDI_SMTick;//Function pointer for the tick.

	// Task 3
	task3.state = -1;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period; // Task current elasped time.
	task3.TickFct = &keypadtest; // Function pointer for the tick.

	// Task 4
	task4.state = -1;//Task initial state.
	task4.period = SMTick4_period;//Task Period.
	task4.elapsedTime = SMTick4_period; // Task current elasped time.
	task4.TickFct = &scroller; // Function pointer for the tick.
	
	// Task 5
	task5.state = -1;//Task initial state.
	task5.period = SMTick5_period;//Task Period.
	task5.elapsedTime = SMTick5_period; // Task current elasped time.
	task5.TickFct = &powerUP; // Function pointer for the tick.

	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	// Scheduler for-loop iterator
	while(1)
	{
		// Scheduler code
		for ( unsigned short i = 0; i < numTasks; i++ )
		{
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period )
			{
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

