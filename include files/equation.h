
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef EQUATION_H
#define EQUATION_H

#include <timer.h>

typedef struct Equation
{
	unsigned char left;
	unsigned char right;
	unsigned char answer; 
	unsigned char* e; 
} E;


E*  load()
{
	srand(internalTimer);
	E temp[100];
	for( int i = 0; i < 100; i++ ) 
	{
		temp[i].left = rand()  %  10; 
		temp[i].right = rand() %  10;
		temp[i].answer = temp[i].right + temp[i].left; 
		strcat( temp[i].e, "              " ); 
		strcpy( temp[i].e, temp[i].left ); 
		strcat( temp[i].e, " + " ); 
		strcat( temp[i].e, temp[i].right ); 
		strcat( temp[i].e, " =                     " ); 
	}
}

#endif //EQUATION_H