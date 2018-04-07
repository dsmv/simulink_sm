
/**

	\mainpage  Программа управления генератором сигналов A7_DAC

	\section Main Основные функции и классы:

	<list type="unnumbered">
	<item> BRDC_main() - точка входа
	<item> TF_SimulinkA7Dac - основной класс теста
	<item> TF_SimulinkShared - обмен с Simulink через разделяемую память
	<item> TL_SharedMemoryWrapper - управление разделяемой памятью
	<item> TF_A7DacCtrl - управление модулем A7_DAC
	<item> TF_PLL - управление синтезатором частоты на модуле A7_DAC
	<item> TF_DDS - управление выводом сигнала через ЦАП на модуле A7_DAC
	</list>
	
	### Назначение параметров S-Function sm_ctrl
	<list type="unnumbered" >
	<item> D0 - канал ЦАП 0 - частота сигнала
	<item> D1 - канал ЦАП 0 - начальная фаза сигнала
	<item> D2 - канал ЦАП 0 - амплитуда сигнала
	<item> D3 - канал ЦАП 1 - частота сигнала
	<item> D4 - канал ЦАП 1 - начальная фаза сигнала
	<item> D5 - канал ЦАП 1 - амплитуда сигнала
	<item> D6 - длина импульса в отсчётах тактовой частоты
	<item> D7 - резерв
	</list>

*/

#include "stdafx.h"
#include <locale.h>

#include "ConsoleTestApi.h"
#include "TableApi.h"


#include "TF_Test.h"
#include "TF_SimulinkA7Dac.h"

U32 g_isTStudio=0;

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


	char *asColumnName[256] = {" TEST_NAME ", "     D0     ", "     D1    ", "     D2    ", "     D3    ", "     D4    ", "     D5    ", "    D6    ", "    D7    " };
	int i, j, k;

	StdoutFlushAndSetBufNull();
	

	// Разбор командной строки
	
	TParse	arParse[] = {
		{_BRDC("tstudio"), &g_isTStudio, PARSE_CMDLINE_DEC },
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


			pTest = new TF_SimulinkA7Dac( argc, argv );
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

