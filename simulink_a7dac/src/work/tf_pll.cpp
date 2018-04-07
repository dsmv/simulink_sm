
//#include <QtTest/QTest>

#include <stdio.h>
#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "tf_pll.h"
#include "cl_reg.h"
//#include    "gipcy.h"

struct TF_PLL_TaskData
{
    //! Доступ к регистрам модуля
    CL_Reg  *reg;

    double  freqSynt;
    double  freqClkA;
    double  freqClkB;

    int     divA;
    int     divB;

};


TF_PLL::TF_PLL(CL_Reg *reg)
{
    td = new TF_PLL_TaskData();

    td->reg = reg;

    td->divA=2;
    td->divB=2;
}

TF_PLL::~TF_PLL()
{
    delete td; td=NULL;

}

//! Подготовка PLL
void TF_PLL::Prepare( void )
{
    printf( "Подготовка PLL\n");

    td->reg->PkgCmdStart();
    td->reg->PkgCmdWrite( 0, 8, 3 );
    td->reg->PkgCmdWrite( 0, 9, 0xF4 );

//    td->reg->PkgCmdWrite( 1, 0, 0x00312815);
//    td->reg->PkgCmdWrite( 1, 0, 0x40870010 );
//    td->reg->PkgCmdWrite( 1, 0, 0x021FE80F);
//    td->reg->PkgCmdWrite( 1, 0, 0x4082C10D);
//    td->reg->PkgCmdWrite( 1, 0, 0x210050CA );
//    td->reg->PkgCmdWrite( 1, 0, 0x03C7C039);
//    td->reg->PkgCmdWrite( 1, 0, 0x207DDBF8);
//    td->reg->PkgCmdWrite( 1, 0, 0x00082317);
//    td->reg->PkgCmdWrite( 1, 0, 0x000004C6);
//    td->reg->PkgCmdWrite( 1, 0, 0x00312805);
//    td->reg->PkgCmdWrite( 1, 0, 0x00000004);
//    td->reg->PkgCmdWrite( 1, 0, 0x201CF3C3 );
//    td->reg->PkgCmdWrite( 1, 0, 0x0FD09002 );
//    td->reg->PkgCmdWrite( 1, 0, 0xC4002051 );
//    td->reg->PkgCmdWrite( 1, 0, 0x40960000 );

    td->reg->PkgCmdExecute();

//    //Sleep( 100 );

//    td->reg->PkgCmdStart();
//    td->reg->PkgCmdWrite( 1, 0, 0x40960000 );
//    td->reg->PkgCmdExecute();

    printf( "Подготовка PLL завершена\n");

    int freq=0;
    int reg0x10=0;
   // for( int ii=0; ii<100; ii++ )
    {
    td->reg->PkgCmdStart();
    td->reg->PkgCmdRead( 0, 0x10, &reg0x10 );
    td->reg->PkgCmdRead( 0, 0x11, &freq );
    td->reg->PkgCmdExecute();


    printf( "reg0x10=0x%.4X  freq=%d \n", reg0x10, freq );
    //for( volatile int jj=0; jj<1000000; jj++ );

    }
}

//! Установка тактовой частоты
int TF_PLL::SetPll(double freq, double *pFreqOut)
{

    double dFreqIn = 100000000.0;

        //
        // Проверить входные параметры
        //
        if( (dFreqIn	< 5.0*1000.0*1000.0) ||
            (dFreqIn	> 900.0*1000.0*1000.0) ||
            (freq	< 47.368*1000.0*1000.0) ||
            (freq	> 3800.0*1000.0*1000.0) )
                return 1;

        //
        // Рассчитать коэффициенты деления R, N, D
        //
        int			rr, nn, dd;
        int			nKeeR, nKeeN, nKeeD;
        int			isBreak;
        double		dFpfd, dFvco, dFclk, dDelta;
        double		dKeeDelta;

        //dKeeDelta = 1.0 * (*pFreqOut);
        dKeeDelta = 1.0 * freq;

        nKeeR = nKeeN = nKeeD = 0;
        isBreak = 0;

        for( rr=1; rr<=255; rr++ )
        {
            dFpfd = dFreqIn / rr;
            if( (dFpfd < 5.0*1000.0*1000.0) ||
                (dFpfd > 200.0*1000.0*1000.0) )
                    continue;
            for( nn=4095; nn>=7; nn-- )
            {
                dFvco = dFpfd * nn;
                if( (dFvco < 1800.0*1000.0*1000.0) ||
                    (dFvco > 3800.0*1000.0*1000.0) )
                        continue;
                for( dd=38; dd>=1; dd-=(dd==2)?1:2 )
                {
                    dFclk = dFvco / dd;
                    dDelta = fabs( dFclk-(freq) );
                    if( dDelta < dKeeDelta )
                    {
                        nKeeR = rr;
                        nKeeN = nn;
                        nKeeD = dd;
                        dKeeDelta = dDelta;
                    }
                    if( 1.0 > dKeeDelta )
                    {
                        isBreak = 1;
                        break;
                    }
                }
                if( isBreak )
                    break;
            }
            if( isBreak )
                break;
        }
        if( 0==nKeeR || 0==nKeeN || 0==nKeeD )
            return 1;

        dFclk = dFreqIn * nKeeN / nKeeR / nKeeD;

        printf( "TF_PLL::SetPll) >>>> R=%d, N=%d, D=%d\n Fclk_req=%15.0f Hz\n Fclk_set=%15.0f Hz\n delta=%17.0f Hz\n",
                   nKeeR, nKeeN, nKeeD, freq, dFclk, dKeeDelta );

        *pFreqOut = dFclk;
        td->freqSynt = dFclk;
        td->freqClkA = td->freqSynt / td->divA;
        td->freqClkB = td->freqSynt / td->divB;

            //
        // Запрограммировать микросхему PLL LMX2581
        //
        unsigned int			nR5, nR3, nR1, nR0, code;

        nR0 = 0x04000000 | (nKeeN<<12);
        nR1 = 0x0C400200 | (nKeeR&0xFF);
        nR3 = 0x02000FBA;
        nR5 = 0x00000A00;
        if( nKeeD > 1 )
        {
            nR3 |= ((nKeeD-2)/2) << 14;
            nR5 |= 1<<7;
        }
        if( dFreqIn<64.0*1000.0*1000.0)	code = 0;
        else if( dFreqIn<128.0*1000.0*1000.0)	code = 1;
        else if( dFreqIn<256.0*1000.0*1000.0)	code = 2;
        else if( dFreqIn<512.0*1000.0*1000.0)	code = 3;
        else code = 4;
        nR5 |= code<<17;

 //       DBG_printf(_BRDC("\n"));

//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x05, 0x00000A81 ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x0F, 0x0021FE80 ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x0D, 0x04082C10 ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x0A, 0x0210050C ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x09, 0x003C7C03 ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x08, 0x0207DDBF ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x07, 0x00008233 ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x06, 0x0000004C ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x05, nR5 ); //	Управление диапазонами частоты опорного сигнала
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x04, 0x00000000 ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x03, nR3 ); //	Значение делителя D
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x02, 0x00FD0900 ); //
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x01, nR1 ); //	Значение делителя R
//        DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x00, nR0 ); //	Значение делителя N


        td->reg->PkgCmdStart();


        SpdWrite( 0x05, 0x0031281 ); //
        SpdWrite( 0x0F, 0x0021FE80 ); //
        SpdWrite( 0x0D, 0x04082C10 ); //
        SpdWrite( 0x0A, 0x0210050C ); //
        SpdWrite( 0x09, 0x003C7C03 ); //
        SpdWrite( 0x08, 0x0207DDBF ); //
        SpdWrite( 0x07, 0x00008233 ); //
        SpdWrite( 0x06, 0x0000004C ); //
        SpdWrite( 0x05, nR5 ); //	Управление диапазонами частоты опорного сигнала
        SpdWrite( 0x04, 0x00000000 ); //
        SpdWrite( 0x03, nR3 ); //	Значение делителя D
        SpdWrite( 0x02, 0x00FD0900 ); //
        SpdWrite( 0x01, nR1 ); //	Значение делителя R
        SpdWrite( 0x00, nR0 ); //	Значение делителя N

        td->reg->PkgCmdExecute();

        //QTest::qSleep( 100 );
		//IPC_delay( 100 );
		Sleep( 100 );

        //RealDelay( 20, 1 );
        //DBG_SpdWrite(pModule, SPDdev_TRDS_SYNT, 0x00, nR0 ); //	Значение делителя N

        td->reg->PkgCmdStart();
        SpdWrite( 0x00, nR0 ); //	Значение делителя N
        td->reg->PkgCmdExecute();

        return 0;
}

//! Запись в регистр синтезатора
void TF_PLL::SpdWrite( unsigned int adr, unsigned int data )
{
     int d = (data << 4 ) | adr;

    td->reg->PkgCmdWrite( 1, 0, d );
}

//! Установка делителей
void TF_PLL::SetDiv( int divA, int divB )
{
    int valA, valB, val;

    switch( divA )
    {
        case 2:     valA = 0; break;
        case 4:     valA = 1; break;
        case 8:     valA = 2; break;
        default:    valA = 3; break;
    }

    switch( divB )
    {
        case 2:     valB = 0; break;
        case 4:     valB = 1; break;
        case 8:     valB = 2; break;
        default:    valB = 3; break;
    }

    val = 0x44 | valB<<4 | valA;

    td->divA=divA;
    td->divB=divB;

    td->freqClkA = td->freqSynt / td->divA;
    td->freqClkB = td->freqSynt / td->divB;

    td->reg->PkgCmdStart();
    td->reg->PkgCmdWrite( 0, 9, val );
    td->reg->PkgCmdExecute();
    printf( "%s  val = 0x%.2X\n", __FUNCTION__, val);
}

//! Установленное значение частоты
double  TF_PLL::GetFreq( int n )
{
    double ret=0;
    switch( n )
    {
        case 0: ret=td->freqSynt/1000000.0; break;
        case 1: ret=td->freqClkA/1000000.0; break;
        case 2: ret=td->freqClkB/1000000.0; break;
    }
    return ret;
}
