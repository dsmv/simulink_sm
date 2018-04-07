#include "utypes.h"

S32 CreateTable(char *pColumnName[], U32 nCount, U08 isTStudio);
S32 AddRowTable();
S32 SetValueTable(U32 nRow, U32 nColumn, char *pVal );
S32 SetValueTable(U32 nRow, U32 nColumn, S32 nVal, char* format=" %d " );
S32 SetValueTable(U32 nRow, U32 nColumn, U32 nVal, char *format=" %u " );
S32 SetValueTable(U32 nRow, U32 nColumn, REAL64 dVal, char *format=" %f " );