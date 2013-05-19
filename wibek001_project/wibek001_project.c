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

enum LCDScroll { CLEAR, SHIFT, PRINT} lcdscroll;
E equations; 
unsigned char scrollCount = 0; 
unsigned char counter = 0; 
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
				
				if( counter > 16 )
				{
					counter = 0; 
					unsigned char left = rand() % 10;
					unsigned char right = rand() % 10;
					char  temp[32];
					strcpy(LCD_string_g, "               ");
					strcat( LCD_string_g, convertFromChar2String(left));
					strcat( LCD_string_g, " + ");
					strcat( LCD_string_g, convertFromChar2String(right));
					strcat( LCD_string_g, "  =                               ");
					//strcpy( LCD_string_g, temp) ; 
				}					
				strcpy( LCD_string_g,  LCD_string_g + 1 );
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
}

enum States { UNHOLD, CAPTURE, HOLD } state; 
unsigned char tempD = 0; 
unsigned char tempA = 0; 
unsigned char pvalue = 0;
unsigned char tp = 0; 
unsigned char t[32] = {"                                "};

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
			LCD_go_g = 0;
			t[tp] = pvalue;
			strcpy( LCD_string_g, t);
			break; 
		case HOLD: 
			LCD_go_g = 1; 
			break; 
		default: 
			break; 
	}
}

int main(void)
{
	init_PWM();
	//equations = load();
	//strcpy( LCD_string_g,  "               2 + 2 =                                   " );
	//strcpy( LCD_string_g, equations[1].e ) ; 
	//LCD_write_str = 0;
	DDRB = 0xFF; // Set port B to output
	DDRC = 0xFF; // Set port D to output
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xF0; PORTD = 0x0F;

	// Period for the tasks
	unsigned long int SMTick1_calc = 100;
	unsigned long int SMTick2_calc = 1;
	unsigned long int SMTick3_calc = 20;

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;

	//Declare an array of tasks
	static task task1, task2, task3;
	task *tasks[] = { &task1, &task2, &task3 };
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
	task3.TickFct = &scroller; // Function pointer for the tick.

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

