

#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdlib.h>
#include <stdint.h>
//#include <inttypes.h>

#include "TF_DDS.h"
#include "cl_reg.h"

struct TF_DDS_TaskData
{
    //! Доступ к регистрам модуля
    CL_Reg  *reg;

};


TF_DDS::TF_DDS(CL_Reg *reg)
{
    td = new TF_DDS_TaskData();

    td->reg = reg;
}

TF_DDS::~TF_DDS()
{
    delete td; td=NULL;

}

//! Подготовка PLL
void TF_DDS::Prepare( int flagLoadSinTable )
{
    printf( "Подготовка DDS\n");

    int regA;
    td->reg->PkgCmdStart();
    td->reg->PkgCmdWrite( 0, 0x80, 0x10 );
    td->reg->PkgCmdRead( 0, 0x0A, &regA );
    td->reg->PkgCmdExecute();

    double v;
    double arg;
    int d;
    int kk=0;

    int flagIsLoaded=0;

    if( 0xCA01==regA)
    {
      printf( " regA=0x%.4X - Таблица уже загружена\n", regA );
    } else
    {
        flagIsLoaded=1;
        printf( " regA=0x%.4X - Требуется загрузка таблицы\n", regA );
    }

//#if 0
    if( flagLoadSinTable || flagIsLoaded )
    {
        printf( "\n Загрузка таблицы\n" );
        for( int jj=0; jj<1024; jj++ )
        {
            td->reg->PkgCmdStart();
            for( int ii=0; ii<16; ii++ )
            {
                arg = 2 * M_PI * kk / 16384;
                v = sin( arg ) * 2047;
                d = v;
                d^=0x800;
                td->reg->PkgCmdWrite( 2, kk, d );
                kk++;
            }
            td->reg->PkgCmdExecute();
        }
        printf( "\n Загрузка таблицы завершена\n" );
        td->reg->PkgCmdStart();
        td->reg->PkgCmdWrite( 0, 0xA, 0xCA01 );
        td->reg->PkgCmdExecute();


    }
//#endif
    td->reg->PkgCmdStart();
    td->reg->PkgCmdWrite( 0, 0x82, 0 );         // PHASE0_START_L
    td->reg->PkgCmdWrite( 0, 0x83, 0x8000 );    // PHASE0_START_H

    td->reg->PkgCmdWrite( 0, 0x84, 0 );         // PHASE0_INC_L
    td->reg->PkgCmdWrite( 0, 0x85, 0x400 );     // PHASE0_INC_H

    td->reg->PkgCmdWrite( 0, 0x86, 0 );         // PHASE1_START_L
    td->reg->PkgCmdWrite( 0, 0x87, 0x4000 );    // PHASE1_START_H

//    td->reg->PkgCmdWrite( 0, 0x88, 0x000100000 );
    td->reg->PkgCmdWrite( 0, 0x88, 0);          // PHASE1_INC_L
    td->reg->PkgCmdWrite( 0, 0x89, 0x2000 );    // PHASE1_INC_H



    td->reg->PkgCmdWrite( 0, 0x91, 0x200 );
    td->reg->PkgCmdWrite( 0, 0x92, 0x1000 );
    td->reg->PkgCmdWrite( 0, 0x93, 0 );

    td->reg->PkgCmdWrite( 0, 0x90, 0x00 );

    td->reg->PkgCmdWrite( 0, 0x80, 0x10 );


    td->reg->PkgCmdExecute();


    printf( "DAC START\n" );
}

void TF_DDS::SetPhase( double syntFreq, double Freq, double Phase, __int64& phase_start, __int64& phase_inc )
{
    long double rate = syntFreq/Freq;
    long double q = 281474976710656.0L;
    phase_inc = q / rate;

    phase_start = (q /360 ) * Phase;

}

//! Установка частоты
void TF_DDS::SetDacFreq( double syntFreq, double Freq0, double Phase0, double Ampl0,  double Freq1, double Phase1, double Ampl1 )
{
    __int64 start0, inc0, start1, inc1;
    SetPhase( syntFreq, Freq0, Phase0, start0, inc0 );
    SetPhase( syntFreq, Freq1, Phase1, start1, inc1 );

    td->reg->PkgCmdStart();

    td->reg->PkgCmdWrite( 0, 0x80, 0x00 );

    td->reg->PkgCmdWrite( 0, 0x82, start0 & 0xFFFFFFFF );         // PHASE0_START_L
    td->reg->PkgCmdWrite( 0, 0x83, (start0>>32) & 0xFFFF );    // PHASE0_START_H

    td->reg->PkgCmdWrite( 0, 0x84, inc0 & 0xFFFFFFFF );         // PHASE0_INC_L
    td->reg->PkgCmdWrite( 0, 0x85, (inc0>>32) & 0xFFFF );     // PHASE0_INC_H

    td->reg->PkgCmdWrite( 0, 0x86, start1 & 0xFFFFFFFF );         // PHASE1_START_L
    td->reg->PkgCmdWrite( 0, 0x87, (start1>>32) & 0xFFFF );    // PHASE1_START_H

    td->reg->PkgCmdWrite( 0, 0x88, inc1 & 0xFFFFFFFF );          // PHASE1_INC_L
    td->reg->PkgCmdWrite( 0, 0x89, (inc1>>32) & 0xFFFF );    // PHASE1_INC_H

    //td->reg->PkgCmdWrite( 0, 0x91, 0x800 );
    //td->reg->PkgCmdWrite( 0, 0x92, 0x1000 );
    //td->reg->PkgCmdWrite( 0, 0x93, 0x100000 );

//    td->reg->PkgCmdWrite( 0, 0x90, 0x07 );

    td->reg->PkgCmdWrite( 0, 0x80, 0x10 );


    td->reg->PkgCmdExecute();
    //printf( "%s  \n", __FUNCTION__ );
    //printf( "  Freq0=%-12.0f Phase0=%-5.0f Ampl0=%-5.0f\n", Freq0, Phase0, Ampl0 );
    //printf( "  Freq1=%-12.0f Phase1=%-5.0f Ampl1=%-5.0f\n", Freq1, Phase1, Ampl1 );

    int inc0_0 = inc0 & 0xFFFFFFFF;
    int inc0_1 = (inc0>>32) & 0xFFFF;

    int inc1_0 = inc1 & 0xFFFFFFFF;
    int inc1_1 = (inc1>>32) & 0xFFFF;

    int start0_0 = start0 & 0xFFFFFFFF;
    int start0_1 = (start0>>32) & 0xFFFF;

    int start1_0 = start1 & 0xFFFFFFFF;
    int start1_1 = (start1>>32) & 0xFFFF;

    //printf( "  inc0= %.4X %.8X   start0=%.4X %.8X\n", inc0_1, inc0_0, start0_1, start0_0);
    //printf( "  inc1= %.4X %.8X   start1=%.4X %.8X\n", inc1_1, inc1_0, start1_1, start1_0);

}


//! Установка счётчиков старта
void TF_DDS::SetCntStart( int regCntEnable, int regCntDisable, int regPeriod, int isContinuePeriod, int isCntEnable )
{
    td->reg->PkgCmdStart();

    int code=0;
    if( isCntEnable )
        code |=1;

    if( isContinuePeriod )
        code |=2;



    td->reg->PkgCmdWrite( 0, 0x90, 0x00 );

    
    td->reg->PkgCmdWrite( 0, 0x91, regCntEnable );
    td->reg->PkgCmdWrite( 0, 0x92, regCntDisable );
    td->reg->PkgCmdWrite( 0, 0x93, regPeriod );

    td->reg->PkgCmdWrite( 0, 0x80, 0x10 );
    td->reg->PkgCmdWrite( 0, 0x90, code );



    td->reg->PkgCmdExecute();

	int ret = isPeriodComplete();
}


//! Завершение вывода
int TF_DDS::isPeriodComplete( void )
{
	int reg0x10=0;
	td->reg->PkgCmdStart();
    td->reg->PkgCmdRead( 0, 0x10, &reg0x10 );

	td->reg->PkgCmdExecute();

	if( reg0x10 & 0x100 )
		return 1;

	return 0;
}

//! Запуск и останов
void TF_DDS::DacStart( int enable, int start )
{
	int code=0x10;
	if( enable )
		code |=0x40;
	if( start )
		code |=0x20;

	td->reg->PkgCmdStart();
    td->reg->PkgCmdWrite( 0, 0x80, code );
	td->reg->PkgCmdExecute();

}


