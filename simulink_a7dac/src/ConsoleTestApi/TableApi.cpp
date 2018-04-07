#include "TableApi.h"
#include <WinCon.h>
#include <string.h>
#include <conio.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <Windows.h>

typedef struct  
{
	int	posX;
	int	len;
}TColumnItem;

TColumnItem *g_paColItems = NULL;
int			g_nWidthTable = 0;
int			g_nColCount = 0;
int			g_nRowCount = 0;
int			g_nYPosition = 0;
U32			g_isTS = 0;

//========================== CreateTable ==========================
//=================================================================
S32 CreateTable(char *pColumnName[], U32 nCount, U08 isTStudio)
{
	int ii;
	HANDLE	hConsoleHandle;
	COORD	dwCursorPosition;
	CONSOLE_SCREEN_BUFFER_INFO rConsoleScreenInfo;
	char	aColName[256] = "";
	
	g_isTS = isTStudio;

	if(g_isTS == 1)
	{
		for(ii = 0; ii < (nCount-1); ii++)
		{
			sprintf(aColName, "%s%s,", aColName, pColumnName[ii]);
		}
		sprintf(aColName, "%s%s", aColName, pColumnName[ii]);
		printf("CREATE TABLE:%s\n", aColName);
		return 0;
	}
	g_nColCount = nCount;

	g_nWidthTable = 1;

	if(g_paColItems != NULL)
		free(g_paColItems);

	g_paColItems = (TColumnItem *) malloc(sizeof(TColumnItem) * nCount);
	if(g_paColItems == NULL)
		return -1;

	for(ii=0; ii < nCount; ii++)
	{
		g_paColItems[ii].posX = g_nWidthTable;
		g_paColItems[ii].len = strlen(pColumnName[ii]);
		g_nWidthTable += g_paColItems[ii].len + 1;
	}

	for(ii = 0; ii < g_nWidthTable; ii++)
		printf("-");
	printf("\n|");
	for(ii = 0; ii < nCount; ii++)
		printf("%s|", pColumnName[ii]);
	printf("\n");
	for(ii = 0; ii < g_nWidthTable; ii++)
		printf("-");
	printf("\n");
	hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleHandle, &rConsoleScreenInfo);
	g_nYPosition = rConsoleScreenInfo.dwCursorPosition.Y;


	return 0;
}

//========================== AddRowTable ==========================
//=================================================================
S32 AddRowTable()
{
	int ii;
	int	jj;
	int	kk;

	if(g_isTS == 1)
	{
		printf("ADD ROW TABLE:\n");
	}
	else
	{
		printf("|");
		for(jj = 0; jj < g_nColCount; jj++)
		{
			for(kk = 0; kk < g_paColItems[jj].len; kk++)
				printf(" ");
			printf("|");
		}
		printf("\n");

		for(ii = 0; ii < g_nWidthTable; ii++)
			printf("-");
		printf("\n");
	}

	g_nRowCount++;

	return g_nRowCount - 1;
}

//========================= SetValueTable =========================
//=================================================================
S32 SetValueTable(U32 nRow, U32 nColumn, char *pVal)
{
	int nValLen;
	int	nLen;
	int ii;
	HANDLE	hConsoleHandle;
	COORD	dwCursorPosition;

	if(g_isTS == 1)
	{
		printf("SET VALUE TABLE:%d,%d,%s\n", nRow, nColumn, pVal);
		return 0;
	}

	if((nRow > g_nRowCount) || (nColumn > g_nColCount))
		return -1;

	nValLen = strlen(pVal);

	if(nValLen > g_paColItems[nColumn].len)
	{
		nLen = g_paColItems[nColumn].len;
		pVal[nLen - 1] = '>';
		pVal[nLen] = '\0';
	}

	dwCursorPosition.X = g_paColItems[nColumn].posX;
	dwCursorPosition.Y = g_nYPosition + nRow*2;

	hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleHandle, dwCursorPosition);

	printf("%s", pVal);

	dwCursorPosition.X = 0;
	dwCursorPosition.Y = g_nYPosition + g_nRowCount*2+1;	

	SetConsoleCursorPosition(hConsoleHandle, dwCursorPosition);
	return 0;
}
S32 SetValueTable(U32 nRow, U32 nColumn, S32 nVal, char *format )
{
	char sStr[256];
	S32 nRet;

	sprintf(sStr, format, nVal);
	nRet = SetValueTable(nRow, nColumn, sStr);	
	return nRet;
}



S32 SetValueTable(U32 nRow, U32 nColumn, U32 nVal, char *format )
{
	char sStr[256];
	S32 nRet;

	sprintf(sStr, format, nVal);
	nRet = SetValueTable(nRow, nColumn, sStr);	
	return nRet;
}


S32 SetValueTable(U32 nRow, U32 nColumn, REAL64 dVal, char *format )
{
	char sStr[256];
	S32 nRet;

	sprintf(sStr, format, dVal);
	nRet = SetValueTable(nRow, nColumn, sStr);
	return nRet;
}