




#include "adc_ctrl.h"



extern U32 g_DelayCnt;

int g_fileMap;
BRD_Handle g_hSRV;
ULONG g_MemAsFifo;
ULONG g_AdcDrqFlag;
ULONG g_MemDrqFlag;
double g_samplRate;
int g_IoDelay;

BRDCHAR g_AdcSrvName[256]; // с номером службы
ULONG g_Cycle;
unsigned __int64 g_samplesOfChannel;
unsigned __int64 g_bBufSize;
int g_DirWriteFile;
ULONG g_FileBufSize;
BRDCHAR g_dirFileName[256];

int g_transRate;
int g_PretrigMode;
long long g_nPostTrigSamples;
