

#include "stdafx.h"
#include <process.h>

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "TF_SimulinkAdc.h"
#include "CL_AMBPEX.h"



#include "ConsoleTestApi.h"
#include "TableApi.h"


#include "adc_ctrl.h"

#include "TF_SimulinkShared.h"

#include "parse_cmd.h"

#include "TF_SourceImit.h"
#include "TF_SourceAdc.h"

#include "global.h"

TF_SimulinkAdc::TF_SimulinkAdc( int argc, BRDCHAR** argv )
{

	BlockWr=0;
	BlockRd=0;
	m_SimulinkState=0;

	//SetDefault();
	//GetParamFromFile( fname );
	//CalculateParams();

	m_SourceMode = GetFromCommnadLine( argc, argv, _BRDC("-mode"), 0 );

	m_Flag2=0;
	m_Flag10=0;

	Terminate=0;
	m_isComplete=0;

	m_pSource = NULL;

	m_pSimulinkShared = new TF_SimulinkShared();

	for( int ii=0; ii<32; ii++ )
	{
		m_pSimulinkShared->SetFlag( ii, 0 );
	}

	m_pSimulinkShared->SetFlag( 1, 0xAA55 );

	switch( m_SourceMode )
	{

		case 1:
			m_pSource = new TF_SourceAdc( argc, argv );
			break;
		case 0:
		default:
			m_pSource = new TF_SourceImit( argc, argv );
			break;
	}


}

TF_SimulinkAdc::~TF_SimulinkAdc()
{
	delete m_pSource;	m_pSource=NULL;
	delete m_pSimulinkShared; m_pSimulinkShared=NULL;
}

void TF_SimulinkAdc::Prepare( void )
{
		int ret;
		for( int ii=0; ; ii++ )
		{
			ret= m_pSource->Prepare(ii);
			if( ret )
				break;
		}
}

void TF_SimulinkAdc::Start( void )
{
	int ii;
	U32 ret;

	m_RowNumber = AddRowTable();

	
	hThread = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, this, CREATE_SUSPENDED, &ThreadId );
	if(hThread == INVALID_HANDLE_VALUE){
		hThread = 0;
	}

  //  SetThreadAffinityMask( hThread, 1 );
	SetThreadPriority( hThread, THREAD_PRIORITY_HIGHEST );


	Sleep( 200 );

	ResumeThread( hThread );

}

void TF_SimulinkAdc::Stop( void )
{
	Terminate=1;

	m_isComplete=1;



}

void TF_SimulinkAdc::Step( void )
{
	U32 adcOvr=0;
	char tname[256];
	sprintf( tname, "Simulink"  );
	SetValueTable( m_RowNumber, 0,  tname );
	SetValueTable( m_RowNumber, 1,  BlockWr, "%10d" );
	SetValueTable( m_RowNumber, 2,  BlockRd, "%10d" );
	//SetValueTable( m_RowNumber, 3,  rd0.BlockOk );
	//SetValueTable( m_RowNumber, 4,  rd0.BlockError );
	//SetValueTable( m_RowNumber, 5,  rd0.VelocityCurrent, "%10.1f" );
	//SetValueTable( m_RowNumber, 6,  rd0.VelocityAvarage, "%10.1f" );

	SetValueTable( m_RowNumber, 3,  m_Flag2, " %.4X" );
	SetValueTable( m_RowNumber, 4,  m_Flag10, "%10d" );
	SetValueTable( m_RowNumber, 5,  m_pSimulinkShared->GetFlag(11), "%10d" );

	SetValueTable( m_RowNumber, 6,  m_pSimulinkShared->GetFlag(20), "%10d" );
	SetValueTable( m_RowNumber, 7,  m_pSimulinkShared->GetFlag(21), "%10d" );


	SetValueTable( m_RowNumber, 8,  m_SimulinkState, "%10d"  );

	//U32 lc_status=0, rm_status=0;

	//sprintf( tname, "%.8X %.2X %.2X", lc_status, rm_status, adcOvr );
	//SetValueTable( m_RowNumber, 8,  tname );

	
}

int TF_SimulinkAdc::isComplete( void )
{
	//if( (lc_status==4) && (IsviStatus==100) )
	if( 1==m_isComplete )
	{
		printf( "\n\nisComplete() - exit\n\n" );
		return 1;
	}
	return 0;
}

void TF_SimulinkAdc::GetResult( void )
{
	
}

UINT  WINAPI	TF_SimulinkAdc::ThreadFunc( LPVOID   lpvThreadParm )
{
	TF_SimulinkAdc *test=(TF_SimulinkAdc*)lpvThreadParm;
	UINT ret;
	if( !test )
		return 0;
	ret=test->Execute();
	return ret;
}
//
////! Установка параметров по умолчанию
//void TF_SimulinkAdc::SetDefault( void )
//{
//	int ii=0;
//
//
//
//	max_item=ii;
//
//   {
//	char str[1024];
//	for( int ii=0; ii<max_item; ii++ )
//	{
//		sprintf( str, "%s  %s", array_cfg[ii].name, array_cfg[ii].def );
//		GetParamFromStr( str );
//	}
//
//
//   }
//
//}
//
////! Расчёт параметров
//void TF_SimulinkAdc::CalculateParams( void )
//{
//
//}

U32 TF_SimulinkAdc::Execute( void )
{
	U32 flag;

	m_pSimulinkShared->SetFlag( 10, 0 );
	m_pSimulinkShared->SetFlag( 11, 0 );

	m_pSimulinkShared->SetFlag( 20, 0 );
	m_pSimulinkShared->SetFlag( 21, 0 );

	m_IndexRd=0;
	m_IndexWr=0;


	for( ; ; )
	{
		if( Terminate )
		{
			break;
		}

		flag=m_pSimulinkShared->GetFlag( 10 );
		m_Flag10=flag;

		// Определение запуска сеанса моделирования
		flag=m_pSimulinkShared->GetFlag( 2 );
		m_Flag2=flag;
		if( 0xB422==flag )
		{

			m_pSimulinkShared->SetFlag( 10, 0 );  // RdReq
			m_pSimulinkShared->SetFlag( 11, 0 );  // RdAck

			m_pSimulinkShared->SetFlag( 20, 0 );  // WrReq
			m_pSimulinkShared->SetFlag( 21, 0 );  // WrAck

			m_pSource->ReStartSession();

			m_IndexRd=0;
			m_IndexWr=0;

			m_pSimulinkShared->SetFlag( 2, 0xB410 );

			m_SimulinkState=10;
		}

		switch( m_SimulinkState )
		{
			case 0:		// Ожидание запуска сеанса моделирования

				Sleep( 100 );
				break;

			case 10:	// Ожидание запуска цикла сбора

				flag=m_pSimulinkShared->GetFlag( 10 );
				m_Flag10=flag;
				if( flag !=m_IndexRd )
				{

					m_pSource->EventStartCycle();

					m_IndexRd++;
					m_pSimulinkShared->SetFlag( 11, m_IndexRd );  // RdAck
					BlockRd++;

					m_SimulinkState = 20;
				}

				
				

				break;
			case 20:	// Ожидание сбора данных

				{
					int ret=1;

					U32 *ptr=NULL;

					ret=m_pSource->GetData( &ptr );

					if( 1==ret )
					{
						// Передача блока данных
						//{
						//	const int blockSize=512;
						//	double *ptr = (double*)(m_pSimulinkShared->GetBuf());
						//	double val;
						//	int k=m_IndexWr%10;
						//	double ampl = 10+m_IndexWr*100;
						//	for( int ii=0; ii<blockSize; ii++ )
						//	{
						//		val = ampl * sin( 2*M_PI*ii/256 );
						//		ptr[ii]=val;
						//	}
						//}

						{
							S16 *src = (S16*)ptr;
							double *dst = (double*)(m_pSimulinkShared->GetBuf());
							double val;
							int blockSize = SIZE_DATA_OUT_ARRAY;

							for( int ii=0; ii<blockSize; ii++ )
							{
								val = *src++;
								*dst++=val;
							}

						}

						// Увеличение индекса
						m_IndexWr++;
						m_pSimulinkShared->SetFlag( 20, m_IndexWr );  // WrReq

						m_SimulinkState = 30;
					}
				}
				break;

			case 30: // Ожидание передачи блока данных
				{
					flag = m_pSimulinkShared->GetFlag( 21 );
				
					if( flag==m_IndexWr )
					{ 
						// Данные обработаны, начинается следующий цикл
						m_SimulinkState = 10;
					}
				}
				break;
		}


		Sleep( 0 );
	}


	m_isComplete=1;
	Sleep( 200 );
	return 1;
}


