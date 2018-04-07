

#include "sm_ctrl.h"
#include "TF_SimulinkShared.h"

//------------------------------------------------------------------------------
void Block::start()
{
    printf("Starting the s-function for outputs testing...\n");


	flagLowVal_0=0;
	flagLowVal_1=0;

	flagError=0;
	indexReq=0;
	indexWr=0;

	m_SharedFileName = getParameterString( FNAME ); 

	m_DataSize = blockSize; // к этому моменту размер выходного массива уже определён



	printf( "DATA_SIZE=%d\n", m_DataSize );
	printf( "FNAME = %s\n", m_SharedFileName.c_str() );

	m_pSimulinkShared = new TF_SimulinkShared( "SimulinkCtrl" );

	U32 timeout=0;
	for( ; ; )
	{
		U32 flag = m_pSimulinkShared->GetFlag( 1 ); // SIG
		if( 0xAA55 == flag )
		{
			break;
		}
		Sleep( 1 );
		timeout++;

		if( timeout>60*1000 )
		{
			flagError=1;
			return;
		}

	}


	m_pSimulinkShared->SetFlag( 2, 0xB422 ); // START
	for( ; ; )
	{
		U32 flag = m_pSimulinkShared->GetFlag( 2 ); // START
		if( 0xB410 == flag )
		{
			// Сеанс моделирования запущен
			break;
		}
		Sleep( 1 );
		timeout++;
		if( timeout>60*1000 )
		{
			flagError=2;
			return;
		}

		flag = m_pSimulinkShared->GetFlag( 1 ); 
		if( 0==flag )
		{
			flagError=21;
			return;
		}


	}
	m_pSimulinkShared->SetFlag( 2, 0xB000 ); // START

}

void Block::outputs()
{

	if( flagError )
		return;

	double start = getInputDouble(START_IN);
//	double get_data = getInputDouble(GET_DATA);

	if( start<0.4 )
	{
		flagLowVal_0=1;
		if( 1==flagHighVal_0 )
		{
			setOutputDouble(START_OUT, start);
			flagHighVal_0=0;
		}
	}

	//if( get_data<0.4 )
	//{
	//	flagLowVal_1=1;
	//}



	initializeSampleTimes();

	if( (start>0.5) && (flagLowVal_0==1) )
	{
		flagLowVal_0=0;

		double sourceParam[8];

		sourceParam[0]=getInputDouble( D0 );
		sourceParam[1]=getInputDouble( D1 );
		sourceParam[2]=getInputDouble( D2 );
		sourceParam[3]=getInputDouble( D3 );
		sourceParam[4]=getInputDouble( D4 );
		sourceParam[5]=getInputDouble( D5 );
		sourceParam[6]=getInputDouble( D6 );
		sourceParam[7]=getInputDouble( D7 );

		for( int ii=0; ii<8; ii++ )
		{
			m_pSimulinkShared->SetDoubleParam( ii, sourceParam[ii] ); 
		}
		

		indexReq++;
		m_pSimulinkShared->SetFlag( 10, indexReq ); 

		U32 timeout=0;
		U32 flag;
		for( ; ; ) 
		{
			flag = m_pSimulinkShared->GetFlag( 11 );
			if( flag==indexReq )
			{
				break;
			}
			timeout++;
			if( timeout>10000 )
			{
				Sleep(1);
			}
			if( timeout>60000 )
			{
				flagError=4;
				break;
			}

			flag = m_pSimulinkShared->GetFlag( 2 ); // START
			if( 0==flag )
			{
				flagError=5; // Перезапуск программы сбора
			}

			flag = m_pSimulinkShared->GetFlag( 1 ); // SIG
			if( 0==flag )
			{
				flagError=6; // Перезапуск программы сбора
			}

			if( flagError )
				break;
		}



		setOutputDouble(START_OUT, start);
		flagHighVal_0 = 1;
	}

}

void Block::terminate()
{
    printf("Terminating the s-function for outputs testing.\n");
}

