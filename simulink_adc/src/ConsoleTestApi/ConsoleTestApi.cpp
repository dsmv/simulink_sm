
#include	"ConsoleTestApi.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<io.h>
#include	<conio.h>

#pragma warning(disable:4996)

void	StdoutFlushAndSetBufNull()
{
	fflush(stdout);
	setbuf(stdout, NULL);
}

// Проверка нажатия клавиши
// Если isFlg равно 1 (приложение запущено из под TStudio), 
// то производится проверка нажатия в TStudio кнопки Stop.
// Вызов далее функции getch() вернет код клавиши Esc.
// Если isFlg равно 0, то производится обычная проверка нажатия клавиши
int kbhit_ex(int isFlg)
{
	BRDCHAR sKey;

	if(isFlg)
	{	// Приложение запущено из под TStudio
		if(_eof(0))
			return kbhit();

		sKey = fgetc(stdin);

		if(sKey == 27)
		{		
			ungetch(27);
			return kbhit();
		}

		return 0;
	}

	return kbhit();
}

//=***************** ParseCommandLine *********************
//=********************************************************
void	ParseCommandLine( int argc, BRDCHAR *argv[], TParse arParse[], int nCnt )
{
	int		ii, jj, idxParam=0;

	for( ii=1; ii<argc; ii++ )
	{
		for( jj=0; jj<nCnt; jj++ )
		{	
			int		len = (int)BRDC_strlen(arParse[jj].name);
			BRDCHAR	*pLin = argv[ii]+1+len;
			BRDCHAR	*endptr;

			if( !BRDC_strnicmp( argv[ii]+1, arParse[jj].name, len ) )
			{
				switch( arParse[jj].type )
				{
				case PARSE_CMDLINE_HEX:	// Hex Value
					if( *(argv[ii]+1+len)=='\0')
					{	ii++;
					pLin = argv[ii];
					}
					*((long int*)arParse[jj].pVal) = BRDC_strtol( pLin, &endptr, 16 );
					break;

				case PARSE_CMDLINE_DEC: // Dec Value
					if( *(argv[ii]+1+len)=='\0')
					{	ii++;
					pLin = argv[ii];
					}
					*((long int*)arParse[jj].pVal) = BRDC_strtol( pLin, &endptr, 0 );
					//sscanf( pLin, "%d", arParse[jj].pVal );
					break;

				case PARSE_CMDLINE_FLOAT: // Floating-point Value
					if( *(argv[ii]+1+len)=='\0')
					{	ii++;
					pLin = argv[ii];
					}
					*((float*)arParse[jj].pVal) = BRDC_strtod( pLin, &endptr);
					//sscanf( pLin, "%lf", arParse[jj].pVal );
					break;

				case PARSE_CMDLINE_STRING: // Character String Value
					if( *(argv[ii]+1+len)=='\0')
					{	ii++;
					pLin = argv[ii];
					}
					BRDC_strcpy( (BRDCHAR*)arParse[jj].pVal, pLin );
					break;

				case PARSE_CMDLINE_TOGGLE: // Toggle Value
					*(unsigned int*)arParse[jj].pVal ^= 1;
					break;
				}
				break;
			}
		}
	}
}

//	Сигнал Start. TStuio, при остановке теста, должна ждать сигнала Stop 
void StartSignal(int isTStudio)
{
	if(isTStudio)
		printf("START SIGNAL:\n");
}

// Сигнал Stop
void StopSignal(int isTStudio)
{
	if(isTStudio)
		printf("STOP SIGNAL:\n");
}