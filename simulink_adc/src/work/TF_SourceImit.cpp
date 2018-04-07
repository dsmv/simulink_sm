
#include "TF_SourceImit.h"

#include "global.h"

#define _USE_MATH_DEFINES
#include <math.h>

struct TF_SourceImit_TaskData 
{
	U32	data_out_size;

	S16	*data;

	int	index;
	int	period;

	TF_SourceImit_TaskData()
	{
		data_out_size = SIZE_DATA_OUT_ARRAY;

		data = new S16[data_out_size];

		index=0;
		period=80;
	}

	~TF_SourceImit_TaskData()
	{
		delete data;
	}
};

TF_SourceImit::TF_SourceImit( int argc, BRDCHAR** argv ) : TF_BaseSource( argc, argv )
{
	BRDC_printf( _BRDC( "Источник данных - имитатор\n") );

	td = new TF_SourceImit_TaskData();
}

TF_SourceImit::~TF_SourceImit()
{
	delete td; td=NULL;
}

//! Prepare function
int TF_SourceImit::Prepare( int count )
{
	if( count>0 )
		return 1;

	BRDC_printf( _BRDC( "Подготовка имитатора данных - Ok\n") );

	return 1;
}

//! Start new session of process data 
void TF_SourceImit::ReStartSession( void )
{
}

//! Start of new data cycle
void TF_SourceImit::EventStartCycle( void )
{
}

//! Get buffer of data
/**
	\param ptr	pointer of pointer of data, output
	\return		1 - set new pointer to ptr
*/
int TF_SourceImit::GetData( U32 **ptr )
{

	S16 *ptrData = td->data;
	S16 val;
	int ampl = 16000;

	int index=td->index;
	int period = td->period;


	for( int ii=0; ii<td->data_out_size; ii++ )
	{
			val = ampl * sin( 2*M_PI*index/period );
			ptrData[ii]=val;
			index++;
	}

	td->index=index;
	*ptr = (U32*)(td->data);
	return 1;
}
