
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef EQUATION_H
#define EQUATION_H

#include <timer.h>

typedef struct Equation
{
	//unsigned char e[100][32];
	//unsigned char* e; 
} E;


E  load()
{
}

unsigned char* convertFromChar2String( unsigned char  num)
{
if( num == 0 ) { return "0"; }
	if( num == 1 ) { return "1"; }
	if( num == 2 ) { return "2"; }
	if( num == 3 ) { return "3"; }	
	if( num == 4 ) { return "4"; }
	if( num == 5 ) { return "5"; }
	if( num == 6 ) { return "6"; }
	if( num == 7 ) { return "7"; }
	if( num == 8 ) { return "8"; }
	if( num == 9 ) { return "9"; }
	if( num == 'A' ) { return "10"; }
	if( num == 'B' ) { return "11"; }
	if( num == 'C' ) { return "12"; }
	if( num == 'D' ) { return "13"; }
	if( num == '*' ) { return "14"; }
	if( num == '#' ) { return "15"; }
	
	return '\0';
}

#endif //EQUATION_H