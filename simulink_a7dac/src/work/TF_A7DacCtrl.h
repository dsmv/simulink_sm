
#ifndef TF_A7DacCtrl_H 

#define TF_A7DacCtrl_H 


#include "TF_BaseSource.h"

struct TF_A7DacCtrl_TaskData;

//! Source data base class 
class TF_A7DacCtrl  : public TF_BaseSource
{

	//! Internal data of TF_A7DacCtrl
	TF_A7DacCtrl_TaskData	*td;

public:

	TF_A7DacCtrl( int argc, BRDCHAR** argv );
	virtual  ~TF_A7DacCtrl();

	//! Prepare function
	virtual int Prepare( int count );

	//! Start new session of process data 
	virtual void ReStartSession( void );

	//! Start of new data cycle
	virtual void EventStartCycle( void );

	void EventStartCycle( int flagChange, double *param );

	int isCycleCompletion( void );
};


#endif


