
#include "TF_SourceAdc.h"

#include "global.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "CL_AMBPEX.h"

#include "adc_ctrl.h"

#include    "gipcy.h"

struct TF_SourceAdc_TaskData 
{
	U32	data_out_size;

	S16	*data;

	int	index;
	int	period;

	CL_AMBPEX	*pBrd;

	BRD_Handle	m_hAdc;

	U32 strmNo;
	U32 StreamBufSizeOfBytes;
	U32 cntBuf;
	U32 cycle;
	U32 trd;


	TF_SourceAdc_TaskData()
	{
		data_out_size = SIZE_DATA_OUT_ARRAY;

		StreamBufSizeOfBytes = data_out_size*2;

		strmNo=0;
		cntBuf=1;
		trd=4;
		cycle=0;

		data = new S16[data_out_size];

		index=0;
		period=80;
		pBrd=NULL;
		m_hAdc=0;
	}

	~TF_SourceAdc_TaskData()
	{
		delete data;
		delete pBrd; pBrd=NULL;
	}
};

TF_SourceAdc::TF_SourceAdc( int argc, BRDCHAR** argv ) : TF_BaseSource( argc, argv )
{
	BRDC_printf( _BRDC( "Источник данных - АЦП \n") );

	td = new TF_SourceAdc_TaskData();

	td->pBrd = new CL_AMBPEX();
}

TF_SourceAdc::~TF_SourceAdc()
{
	td->pBrd->StreamDestroy( td->strmNo );
	delete td; td=NULL;
}

//! Prepare function
int TF_SourceAdc::Prepare( int count )
{
	if( count>0 )
		return 1;

	BRDC_printf( _BRDC( "Подготовка АЦП\n") );

	td->pBrd->init( 1 );

	PrepareAdc( "exam_adc.ini" );

	td->pBrd->StreamInit( td->strmNo, td->cntBuf, td->StreamBufSizeOfBytes, td->trd, 1,  td->cycle, 0, 0 );
	BRDC_printf( _BRDC( "Подготовка АЦП завершена\т\n") );

	return 1;
}

//! Start new session of process data 
void TF_SourceAdc::ReStartSession( void )
{
	td->pBrd->StreamStop( td->strmNo );

	IPC_delay( 100 );

}

//! Start of new data cycle
void TF_SourceAdc::EventStartCycle( void )
{
		td->pBrd->StreamStart( td->strmNo );
		td->pBrd->RegPokeInd( td->trd, 0, 0x2038 );

}

//! Get buffer of data
/**
	\param ptr	pointer of pointer of data, output
	\return		1 - set new pointer to ptr
*/
int TF_SourceAdc::GetData( U32 **ptr )
{

	int ret;
	U32 *pStreamData;

	ret=td->pBrd->StreamGetBuf( td->strmNo, &pStreamData );

	if( 0==ret )
		return 0;

	U32 *ptrData = (U32*)(td->data);

	for( int ii=0; ii<td->data_out_size/2; ii++ )
	{
			ptrData[ii]=pStreamData[ii];
	}

	*ptr = (U32*)(td->data);
	return 1;
}



//! Подготовка FM214x250M
void TF_SourceAdc::PrepareAdc( char* fname )
{
	U32 atr;

   BRDC_printf( _BRDC("\nФайл инициализации АЦП: ") );
   printf( "%s \n", fname );
   char srvName[210];
   char srvName1[210];
   
   srvName[0]=0;

	{
	  // Определение имени службы АЦП
	  FILE *in = fopen( fname, "rt" );
	  if( NULL==in )
	  {
		  throw( _BRDC("Ошибка при открытии файла") );
	  }
	  char str[512];
	  for( ; ; )
	  {
		  if( fgets( str, 200, in )==NULL )
			  break;
		  if( strncmp( "AdcServiceName=", str, 15 )==0 )
		  {
			sscanf( str+15, "%s", srvName );
			break;
		  }

	  }
	  sprintf( srvName1, "%s0", srvName );

	}


#ifdef UNICODE
	
BRDCHAR _srvName[512];
BRDCHAR _srvName1[512];

	 MultiByteToWideChar(
			CP_ACP,			//	  __in   UINT CodePage,
			MB_PRECOMPOSED,	//	  __in   DWORD dwFlags,
			srvName,			//	  __in   LPCSTR lpMultiByteStr,
			-1,				//	  __in   int cbMultiByte,
			_srvName,			//	 __out  LPWSTR lpWideCharStr,
			512				//	__in   int cchWideChar
	);

	 MultiByteToWideChar(
			CP_ACP,			//	  __in   UINT CodePage,
			MB_PRECOMPOSED,	//	  __in   DWORD dwFlags,
			srvName1,			//	  __in   LPCSTR lpMultiByteStr,
			-1,				//	  __in   int cbMultiByte,
			_srvName1,			//	 __out  LPWSTR lpWideCharStr,
			512				//	__in   int cchWideChar
	);

	td->m_hAdc=td->pBrd->GetSrvHandle( _srvName1, &atr );
#else
	m_hAdc=pBrd->GetSrvHandle( srvName1, &atr );
#endif

	BRDC_printf( _BRDC("\n#####Служба АЦП: %hs %.8X \n"), srvName1, td->m_hAdc );

	//m_hAdc=pBrd->GetSrvHandle( _BRDC("FM214X250M0"), &atr );

	if( !td->m_hAdc )
	{
	   throw( _BRDC("Служба АЦП не найдена ") );
	}

   BRDC_printf( _BRDC("\nСлужба АЦП: %.8X \n"), td->m_hAdc );

   if( NULL==fname )
   {
	   throw( _BRDC( "Не задано имя файла инициализации АЦП " ));
   }


   //AdcSettings( m_hAdc, 0, _BRDC("FM214X250M0"), fname );
   //AdcSettings( m_hAdc, 0, _BRDC("FM214X250M0"), _BRDC("\\exam_adc.ini") );
   
   char fname1[512];
   sprintf( fname1, "\\%s", fname );

#ifdef UNICODE
	
BRDCHAR _fname[512];

	 MultiByteToWideChar(
			CP_ACP,			//	  __in   UINT CodePage,
			MB_PRECOMPOSED,	//	  __in   DWORD dwFlags,
			fname1,			//	  __in   LPCSTR lpMultiByteStr,
			-1,				//	  __in   int cbMultiByte,
			_fname,			//	 __out  LPWSTR lpWideCharStr,
			512				//	__in   int cchWideChar
	);

	AdcSettings( td->m_hAdc, 0, _srvName, _fname );
#else
	
	AdcSettings( m_hAdc, 0, srvName, fname1 );
   //AdcSettings( m_hAdc, 0, srvName, "\\exam_adc.ini" );
#endif

   
}