

#include "stdafx.h"
#include <process.h>

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>

#define _USE_MATH_DEFINES
#include <math.h>


#include "ConsoleTestApi.h"
#include "TableApi.h"

#include "TF_SimulinkA7Dac.h"

#include "TF_SimulinkShared.h"

#include "parse_cmd.h"

#include "global.h"

#include "TF_A7DacCtrl.h"

TF_SimulinkA7Dac::TF_SimulinkA7Dac( int argc, BRDCHAR** argv )
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

	m_pSource = new TF_A7DacCtrl( argc, argv );

	m_pSimulinkShared = new TF_SimulinkShared( "SimulinkCtrl" );

	for( int ii=0; ii<32; ii++ )
	{
		m_pSimulinkShared->SetFlag( ii, 0 );
	}

	for( int ii=0; ii<8; ii++ )
	{
		m_SimulinkParam[ii]=0;
		m_pSimulinkShared->SetDoubleParam( ii, 0 );
	}
	m_pSimulinkShared->SetFlag( 1, 0xAA55 );


}

TF_SimulinkA7Dac::~TF_SimulinkA7Dac()
{
	delete m_pSource; m_pSource=NULL;
	delete m_pSimulinkShared; m_pSimulinkShared=NULL;
}

void TF_SimulinkA7Dac::Prepare( void )
{
		int ret=1;
		for( int ii=0; ; ii++ )
		{
			ret= m_pSource->Prepare(ii);
			if( ret )
				break;
		}
}

void TF_SimulinkA7Dac::Start( void )
{
	int ii;
	U32 ret;

	m_RowNumber = AddRowTable();
	m_RowNumber2 = AddRowTable();

	
	hThread = (HANDLE)_beginthreadex( NULL, 0, ThreadFunc, this, CREATE_SUSPENDED, &ThreadId );
	if(hThread == INVALID_HANDLE_VALUE){
		hThread = 0;
	}

  //  SetThreadAffinityMask( hThread, 1 );
	SetThreadPriority( hThread, THREAD_PRIORITY_HIGHEST );


	Sleep( 200 );

	ResumeThread( hThread );

}

void TF_SimulinkA7Dac::Stop( void )
{
	Terminate=1;

	m_isComplete=1;



}

void TF_SimulinkA7Dac::Step( void )
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

	SetValueTable( m_RowNumber, 3,  m_Flag2, "   %.4X" );
	SetValueTable( m_RowNumber, 4,  m_Flag10, "%10d" );
	SetValueTable( m_RowNumber, 5,  m_pSimulinkShared->GetFlag(11), "%10d" );

	SetValueTable( m_RowNumber, 6,  m_pSimulinkShared->GetFlag(20), "%10d" );
	SetValueTable( m_RowNumber, 7,  m_pSimulinkShared->GetFlag(21), "%10d" );


	SetValueTable( m_RowNumber, 8,  m_SimulinkState, "%10d" );


	SetValueTable( m_RowNumber2, 1,  m_SimulinkParam[0], "%10g" );
	SetValueTable( m_RowNumber2, 2,  m_SimulinkParam[1], "%10g" );
	SetValueTable( m_RowNumber2, 3,  m_SimulinkParam[2], "%10g" );
	SetValueTable( m_RowNumber2, 4,  m_SimulinkParam[3], "%10g" );
	SetValueTable( m_RowNumber2, 5,  m_SimulinkParam[4], "%10g" );
	SetValueTable( m_RowNumber2, 6,  m_SimulinkParam[5], "%10g" );
	SetValueTable( m_RowNumber2, 7,  m_SimulinkParam[6], "%10g" );
	SetValueTable( m_RowNumber2, 8,  m_SimulinkParam[7], "%10g" );

	//U32 lc_status=0, rm_status=0;

	//sprintf( tname, "%.8X %.2X %.2X", lc_status, rm_status, adcOvr );
	//SetValueTable( m_RowNumber, 8,  tname );

	
}

int TF_SimulinkA7Dac::isComplete( void )
{
	//if( (lc_status==4) && (IsviStatus==100) )
	if( 1==m_isComplete )
	{
		printf( "\n\nisComplete() - exit\n\n" );
		return 1;
	}
	return 0;
}

void TF_SimulinkA7Dac::GetResult( void )
{
	
}

UINT  WINAPI	TF_SimulinkA7Dac::ThreadFunc( LPVOID   lpvThreadParm )
{
	TF_SimulinkA7Dac *test=(TF_SimulinkA7Dac*)lpvThreadParm;
	UINT ret;
	if( !test )
		return 0;
	ret=test->Execute();
	return ret;
}
//
////! Установка параметров по умолчанию
//void TF_SimulinkA7Dac::SetDefault( void )
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
//void TF_SimulinkA7Dac::CalculateParams( void )
//{
//
//}

U32 TF_SimulinkA7Dac::Execute( void )
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

			for( int ii=0; ii<8; ii++ )
			{
				m_SimulinkParam[ii]=0;
			}

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

					double param[8];
					int flagChangeParam=0;
					// Чтение параметров
					for( int ii=0; ii<8; ii++ )
					{
						param[ii]=m_pSimulinkShared->GetDoubleParam( ii );
						if( param[ii]!=m_SimulinkParam[ii] )
							flagChangeParam=1;

					}

					if( flagChangeParam )
					{
						for( int ii=0; ii<8; ii++ )
						{
							m_SimulinkParam[ii]=param[ii];
						}
						BlockWr++;
					}

					m_pSource->EventStartCycle( flagChangeParam, param );

					m_IndexRd++;
					m_pSimulinkShared->SetFlag( 11, m_IndexRd );  // RdAck
					BlockRd++;

					m_SimulinkState = 11;
				}

				break;

			case 11:  // Ожидание завершения цикла вывода
				{
					int ret=m_pSource->isCycleCompletion();
					if( ret )
					{
						m_SimulinkState = 10;
						break;
					}
					Sleep( 1 );
				}
				break;

		
		}


		Sleep( 0 );
	}


	m_isComplete=1;
	Sleep( 200 );
	return 1;
}


