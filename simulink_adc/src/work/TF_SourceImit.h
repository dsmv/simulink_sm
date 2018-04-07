
#ifndef TF_SourceImit_H 

#define TF_SourceImit_H 


#include "TF_BaseSource.h"

struct TF_SourceImit_TaskData;

/**
		\brief Имитатор данных

		Класс формирует синусоиду.
		Класс предназначен для проверки взаимодействия с Simulink

*/
class TF_SourceImit  : public TF_BaseSource
{

	//! Internal data of TF_SourceImit
	TF_SourceImit_TaskData	*td;

public:

	TF_SourceImit( int argc, BRDCHAR** argv );
	virtual  ~TF_SourceImit();

	//! Prepare function
	virtual int Prepare( int count );

	//! Start new session of process data 
	virtual void ReStartSession( void );

	//! Start of new data cycle
	virtual void EventStartCycle( void );

	//! Get buffer of data
	/**
		\param ptr	pointer of pointer of data, output
		\return		1 - set new pointer to ptr
	*/
	virtual int GetData( U32 **ptr );

};


#endif


