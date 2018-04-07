
#include "TF_A7DacCtrl.h"

#include "global.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "tf_a7dac.h"
#include "tf_pll.h"
#include "tf_dds.h"


struct TF_A7DacCtrl_TaskData 
{

	int	index;
	int	period;

    TF_A7Dac    a7dac;
    TF_PLL      *pll;
    TF_DDS      *dds;

	int flagStart;

	TF_A7DacCtrl_TaskData()
	{

        pll=NULL;
        dds=NULL;

		index=0;
		period=80;
		
		flagStart=0;
	}

	~TF_A7DacCtrl_TaskData()
	{
        delete pll; pll=NULL;
        delete dds; dds=NULL;
	}
};

TF_A7DacCtrl::TF_A7DacCtrl( int argc, BRDCHAR** argv ) : TF_BaseSource( argc, argv )
{

	td = new TF_A7DacCtrl_TaskData();
}

TF_A7DacCtrl::~TF_A7DacCtrl()
{
	delete td; td=NULL;
}

//! Prepare function
int TF_A7DacCtrl::Prepare( int count )
{
	if( count>0 )
		return 1;

	BRDC_printf( _BRDC( "Подготовка A7Dac - Ok\n") );

 td->a7dac.Open();

    //td->a7dac.PldLoad( "a7dac_m2.bit" );

    td->pll = new TF_PLL( &(td->a7dac) );
    td->dds = new TF_DDS( &(td->a7dac) );

    td->a7dac.PkgCmdStart();
    td->a7dac.PkgCmdWrite( 0, 8, 0x01020304 );
    td->a7dac.PkgCmdWrite( 0, 9, 0xA9A8A7A6 );
    td->a7dac.PkgCmdExecute();


    int reg0=-1;
    int reg1=-1;
    int sig0=-1;
    int sig1=-1;
    int sig2=-1;
    int lmx_sig=-1;
    int dpram_sig=-1;

    td->a7dac.PkgCmdStart();
    td->a7dac.PkgCmdRead( 0, 0, &sig0 );
    td->a7dac.PkgCmdRead( 0, 1, &sig1 );
    td->a7dac.PkgCmdRead( 0, 2, &sig2 );
    td->a7dac.PkgCmdRead( 0, 8, &reg0 );
    td->a7dac.PkgCmdRead( 0, 9, &reg1 );
   /*  */

    td->a7dac.PkgCmdRead( 1, 0, &lmx_sig );

   // td->a7dac.PkgCmdRead( 2, 0, &dpram_sig );

    td->a7dac.PkgCmdExecute();

    printf( "sig0 = 0x%.8X\n", sig0 );
    printf( "sig1 = 0x%.8X\n", sig1 );
    printf( "sig2 = 0x%.8X\n", sig2 );

    printf( "reg0 = 0x%.8X\n", reg0 );
    printf( "reg1 = 0x%.8X\n", reg1 );

    printf( "lmx_sig = 0x%.8X\n", lmx_sig );
    printf( "dpram_sig = 0x%.8X\n", dpram_sig );

    td->pll->Prepare();

	td->pll->SetDiv( 2, 2 );
    double FreqPll=0;
    td->pll->SetPll( 1000000000.0, &FreqPll );
    printf( "\nFreqPll: %g\n", FreqPll );

    td->dds->Prepare(0);

	double param[8];
	param[0]=10;
	param[1]=0;
	param[2]=16000;
	param[3]=10;
	param[4]=0;
	param[5]=16000;

	param[6]=2500;
	int pulse=param[6];
	double syntFreq = td->pll->GetFreq(1) * 1000000.0;
	td->dds->SetDacFreq( syntFreq, param[0]*1000000.0, param[1], param[2], param[3]*1000000.0, param[4], param[5] );
	td->dds->SetCntStart( pulse, 10, 0, 0, 1 );


	//for( ; ; )
	//{
	//	int ret;
	//	td->dds->DacStart( 1, 1 );

	//	for( ; ; ) 
	//	{
	//		ret = td->dds->isPeriodComplete();
	//		if( ret )
	//			break;
	//	}
	//	
	//	td->dds->DacStart( 1, 0 );
	//	Sleep( 100 );
	//}

	return 1;
}

//! Start new session of process data 
void TF_A7DacCtrl::ReStartSession( void )
{
	td->flagStart=1;

	td->dds->DacStart( 0, 0 );
}

//! Start of new data cycle
void TF_A7DacCtrl::EventStartCycle( void )
{
}

void TF_A7DacCtrl::EventStartCycle( int flagChange, double *param )
{
	if( flagChange )
	{
		double syntFreq = td->pll->GetFreq(1) * 1000000.0;
		td->dds->SetDacFreq( syntFreq, param[0]*1000000.0, param[1], param[2], param[3]*1000000.0, param[4], param[5] );
		td->dds->SetCntStart( param[6], 10, 0, 0, 1 );
	}

	td->dds->DacStart( 1, 1 );
	td->flagStart=0;
}

int TF_A7DacCtrl::isCycleCompletion( void )
{
	int ret = td->dds->isPeriodComplete();
	if( ret )
	{
		td->dds->DacStart( 1, 0 );
	}
	return ret;
}


