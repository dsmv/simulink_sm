
#ifndef TF_BASESOURCE_H 

#define TF_BASESOURCE_H 


#include "brd.h"


//! Source data base class 
class TF_BaseSource  
{

public:

	TF_BaseSource( int argc, BRDCHAR** argv ) {}
	virtual  ~TF_BaseSource() {}

	//! Prepare function
	virtual int Prepare( int count ) { return 1; };

	//! Start new session of process data 
	virtual void ReStartSession( void ) {};

	//! Start of new data cycle
	virtual void EventStartCycle( void ) {};

	//! Get buffer of data
	/**
		\param ptr	pointer of pointer of data, output
		\return		1 - set new pointer to ptr
	*/
	virtual int GetData( U32 **ptr ) { return 0; };

};


#endif


