#include "utypes.h"

typedef struct { BRDCHAR name[10]; void *pVal; int type; } TParse;

enum
{
	PARSE_CMDLINE_HEX		=	0,
	PARSE_CMDLINE_DEC		=	1,
	PARSE_CMDLINE_FLOAT		=	2,
	PARSE_CMDLINE_64_BIT	=	3,
	PARSE_CMDLINE_STRING	=	4,
	PARSE_CMDLINE_TOGGLE	=	5,
};

void	StdoutFlushAndSetBufNull();
// �������� ������� �������
int		kbhit_ex(int isFlg);
void	ParseCommandLine( int argc, BRDCHAR *argv[], TParse arParse[], int nCnt );
//	������ Start. TStuio, ��� ��������� �����, ������ ����� ������� Stop 
void	StartSignal(int isTStudio);
// ������ Stop
void	StopSignal(int isTStudio);