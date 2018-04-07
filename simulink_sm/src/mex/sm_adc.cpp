//------------------------------------------------------------------------------
/* C++ S-function that shows all the different ways to use the output ports.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfun_outputs.cpp
 *
 */

#include "sm_adc.h"
#include "TF_SimulinkShared.h"

//------------------------------------------------------------------------------
void Block::start()
{
    printf("Starting the s-function for outputs testing...\n");

	//waveform = new double[blockSize];
	//cnt=10;
	//flag=0;
	flagLowVal_0=0;
	flagLowVal_1=0;

	flagError=0;
	indexReq=0;
	indexWr=0;

	m_DataSize = getParameterInt( DATA_SIZE );
	m_SharedFileName = getParameterString( FNAME ); 

	m_DataSize = blockSize; // к этому моменту размер выходного массива уже определён

	//if( m_DataSize<1 || m_DataSize>256*1024 )
	//{
	//	m_DataSize=512;
	//}
	//outputPortRows[0]=m_DataSize;

	printf( "DATA_SIZE=%d\n", m_DataSize );
	printf( "FNAME = %s\n", m_SharedFileName.c_str() );

	m_pSimulinkShared = new TF_SimulinkShared( "SimulinkAdc" );

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
	double get_data = getInputDouble(GET_DATA);

	if( start<0.4 )
	{
		flagLowVal_0=1;
		if( 1==flagHighVal_0 )
		{
			setOutputDouble(START_OUT, start);
			flagHighVal_0=0;
		}
	}

	if( get_data<0.4 )
	{
		flagLowVal_1=1;
	}

    //printf("---------- time = %f ----------\n",ssGetT(simStruct));
    //printf("output port %i = 7\n",DATA);
    //setOutputDouble(OUT1,7);

    //out2.init(1);
    //out2[1]=2;
    //out2.print();
    //setOutputArray(OUT2,out2);

    //out3.init(3);
    //out3(0,2)=4;
    //out3.print();
    //setOutputArray(OUT3,out3);

	initializeSampleTimes();

	if( (start>0.5) && (flagLowVal_0==1) )
	{
		flagLowVal_0=0;

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
		}




		setOutputDouble(START_OUT, start);
		flagHighVal_0 = 1;
	}

	if( (get_data>0.5) && (flagLowVal_1==1 ))
	{
		flagLowVal_1=0;
		Array  outArray( m_DataSize, 1 );

		U32 timeout=0;
		U32 flag;
		for( ; ; ) 
		{
			flag = m_pSimulinkShared->GetFlag( 20 );
			if( flag!=indexWr )
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
				flagError=5;
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
		}
		// Получение данных
		{
			double *ptr = (double*)(m_pSimulinkShared->GetBuf());
			for( int ii=0; ii<m_DataSize; ii++ )
			{
				outArray[ii]=*ptr++;
			}
		}


		indexWr++;
		m_pSimulinkShared->SetFlag( 21, indexWr ); 

		//if( 0==flag )
		//{
		//	for( int ii=0; ii<blockSize; ii++ )
		//	{
		//		outArray[ii]=ii%cnt;
		//	}
		//	flag=1;
		//} else
		//{
		//	for( int ii=0; ii<blockSize; ii++ )
		//	{
		//		outArray[ii]=(blockSize-ii-1)%cnt;
		//	}
		//	flag=0;
		//}

		setOutputArray(DATA,outArray);

	}

		

	//data = cnt++;
	//setOutputDouble(DATA, data);
	//if( cnt==blockSize )
	//{
	//	cnt=0;
	//}
}

void Block::terminate()
{
    printf("Terminating the s-function for outputs testing.\n");
//	delete[] waveform;
}

