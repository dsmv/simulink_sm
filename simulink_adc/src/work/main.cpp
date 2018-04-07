
/**

	\mainpage  Программа приёма данных от АЦП

	\section Main Основные функции и классы:

	<list type="unnumbered">
	<item> BRDC_main() - точка входа
	<item> TF_SimulinkAdc - основной класс теста
	<item> TF_SimulinkShared - обмен с Simulink через разделяемую память
	<item> TL_SharedMemoryWrapper - управление разделяемой памятью
	<item> TF_BaseSource - базовый класс источника сигналов
	<item> TF_SourceImit - источник данных - имитатора
	<item> TF_SimulinkAdc - источник данных - модуль АЦП
	</list>
	

*/

#include "stdafx.h"
#include <locale.h>

#include "ConsoleTestApi.h"
#include "TableApi.h"

#include "CL_AMBPEX.h"

#include "TF_Test.h"
#include "TF_SimulinkAdc.h"


U08			g_isTStudio = 0;

//
//=== Console
//
HANDLE	hConsoleOut;


int BRDC_main(int argc, BRDCHAR* argv[])
{

	int isFirstCallStep=1;
	COORD rCursorPosition;

	// анализ командной строки
	setlocale( LC_ALL, "Russian" );

	//=== Console Settings
	{
		SMALL_RECT rWindow;
		COORD rCoord;
		BOOL bret;
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

		hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );

		GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

		rCoord=csbiInfo.dwSize;
		rCoord.X=120;
		
		rWindow=csbiInfo.srWindow;
		rWindow.Right = rCoord.X-1;   
		

		bret=SetConsoleScreenBufferSize(hConsoleOut, rCoord);
		bret=SetConsoleWindowInfo( hConsoleOut, TRUE, &rWindow  );
	}


	char *asColumnName[256] = {" TEST_NAME ", "     S0     ", "     S1    ", "     S2    ", "     S3    ", "     S4    ", "     S5    ", "    S6    ", "    S7    " };

	int i, j, k;

	StdoutFlushAndSetBufNull();
	
	// Разбор командной строки
	
	TParse	arParse[] = {
		{_BRDC("tstudio"), &g_isTStudio, PARSE_CMDLINE_DEC }
	};
	

	int nCnt = sizeof(arParse)/sizeof(TParse);

	ParseCommandLine(argc, argv, arParse, nCnt);

	printf("LOG:Start test.\n");

	StartSignal(g_isTStudio);

	TF_Test  *pTest=NULL;
	TF_Test  *pTest2=NULL;



	try
	{
			int ret;


			CL_AMBPEX *pBrd = NULL;


			pTest = new TF_SimulinkAdc( argc, argv );
			pTest2 = NULL;
			

			pTest->Prepare();
			if( pTest2 ) 
				pTest2->Prepare();



			CreateTable(asColumnName, 9, g_isTStudio);

			Sleep( 10 );
			pTest->Start();
			Sleep( 10 );
			if( pTest2 )
				pTest2->Start();


			//int key;
			for( ; ; )
			{
				if( kbhit_ex( g_isTStudio ) )
				{
					int key=_getch(); 
					if( key==0x1B )
					{
						printf("LOG:Stop test.\n");
						pTest->Stop();
						if( pTest2 )
							pTest2->Stop();
						BRDC_printf( _BRDC("\n\nОтмена\n") );
						Sleep( 100 );
					}



				}
				ret=pTest->isComplete();
				if( ret )
				{
					BRDC_printf( _BRDC("\n\nВыход\n") );
					if( pTest2 )
					{
						ret=pTest2->isComplete();
						if( ret )
							break;
					} else
					{
					   break;
					}
					
				}



				pTest->Step();
				if( pTest2 )
					pTest2->Step();
				Sleep( 100 );
			}
			pTest->GetResult();
			if( pTest2 )
				pTest2->GetResult();


			delete pTest; pTest=NULL;
			delete pTest2; pTest2=NULL;

			//delete pBrd; pBrd=NULL;

	}
	catch( BRDCHAR* str )
	{
		BRDC_printf( _BRDC("ERROR: %s \n"), str );
	}
	catch( ... )
	{
		BRDC_printf( _BRDC("ERROR: Неизвестная ошибка выполнения программы\n") );
	}


	if( 0==g_isTStudio )
	{
	 printf( "\n Press any key\n" );
	 _getch();
	}
	
	StopSignal(g_isTStudio);

	return 0;
}

