
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef TONE_H
#define TONE_H

//C = 261.63 Hz
//D = 293.66 Hz
//E = 329.63 Hz
//F = 349.23 Hz
//G = 392.00 Hz
//A = 440.00 Hz
//B = 493.88 Hz

double tone(int t )
{
	if( t == 0  )
	{
		return 0 ;
	}
	else if ( t == 1 )
	{
		return 277.66 ;//d
	}
	else if ( t == 2 )
	{
		return 300.63;//f#
	}
	else if ( t == 3 )
	{
		return 295.23 ;//f
	}
	else if ( t == 4 )
	{
		return 307.00 ;//g
	}
	else if ( t == 5 )
	{
		return 317.00 ;//a
	}
	else if ( t == 6 )
	{
		return 327.88 ;//b
	}
	else if ( t == 7 )
	{
		return 337.88;//c
	}
	else if ( t == 8 )
	{
		return 261.63;// lower c  
	}
	else if ( t == 9 )
	{
		return 266.63; //lowerc#
	}
	else if ( t == 10 )
	{
		return 248.63; //lower b 
	}
	else if ( t == 11 )
	{
		return 287.63; // e
	}
	else if ( t == 12 )
	{
		return 245.63; // lower g 
	}
	else if ( t == 13 )
	{
		return 255.63; // lower g
	}
	else
	{
		return 0;
	}		
}


#endif //TONE_H