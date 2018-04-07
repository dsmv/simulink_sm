//****************** File icr.h ***************************
//
//  Определения констант и структур 
//	для работы с конфигурационным ППЗУ
//
//  Constants & structures definitions
//	for identification & configuration EPROM
//
//	Copyright (c) 2002-2003, Instrumental Systems,Corp.
//	Written by Dorokhin Andrey
//
//  History:
//  15-01-03 - builded
//  31-07-03 - add ICR_Cfg0310 Submodule DAC configuration structure
//  09-09-03 - add ICR_Cfg0510 Host PLD configuration structure
//  09-09-03 - add ICR_Cfg0610 ADM2 interface configuration structure

//
//*********************************************************

#ifndef __ICR_H_
 #define __ICR_H_

#include "utypes.h"

#pragma pack(push, 1)    

const U16 END_TAG			= 0x0000; // tag of end data (тэг, информирующий об окончании данных)
const U16 ALT_END_TAG		= 0xffff; // alt tag of end data (альтернативный тэг, информирующий об окончании данных)

const U16 BASE_ID_TAG		= 0x4953; // tag of base module identification structure
const U16 AMBPCI_CFG_TAG	= 0x4D50; // tag of AMBPCI configuration structure 

const U16 ADM_ID_TAG		= 0x0080; // tag of ADM submodule identification structure

const U16 ADC_CFG_TAG		= 0x0100; // tag of ADC configuration structure
const U16 ADC_FIFO_TAG		= 0x0200; // tag of ADC FIFO configuration structure
const U16 DAC_CFG_TAG		= 0x0300; // tag of DAC configuration structure
const U16 ADMDAC_CFG_TAG	= 0x0310; // tag of Submodule DAC configuration structure
const U16 DAC_FIFO_TAG		= 0x0400; // tag of DAC FIFO configuration structure
const U16 PLD_CFG_TAG		= 0x0500; // tag of PLD configuration structure
const U16 HOSTPLD_CFG_TAG	= 0x0510; // tag of HOST PLD configuration structure
const U16 ADMIF_CFG_TAG		= 0x0600; // tag of ADM-interface configuration structure
const U16 ADM2IF_CFG_TAG	= 0x0610; // tag of ADM2-interface configuration structure

const U16 SDRAM_CFG_TAG		= 0x0700; // тэг для структуры конфигурационных параметров динамической памяти

const U16 SUBMOD_CFGMEM_SIZE = 512; // размер ППЗУ, устанавливаемом на субмодуль

// Base module identification
// Идентификационная структура базового модуля
typedef struct _ICR_Id4953 {
	U16	wTag;		// tag of structure (BASE_ID_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U16	wSizeAll;	// size of all data, writing into base module EPROM
	U32	dSerialNum;	// serial number (серийный (физический) номер)
	U16	wDeviceId;	// base module type (тип базового модуля)
	U08	bVersion;	// base module version (версия базового модуля)
	U08	bDay;		// day of Data (день записи данных в ППЗУ базового модуля)
	U08	bMon;		// montag of Data (месяц записи данных в ППЗУ базового модуля)
	U16	wYear;		// year of Data (год записи данных в ППЗУ базового модуля)
} ICR_Id4953, *PICR_Id4953, ICR_IdBase, *PICR_IdBase;

// ADM identification
// Идентификационная структура субмодуля
typedef struct _ICR_Id0080 {
	U16	wTag;		// tag of structure (ADM_ID_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U16	wSizeAll;	// размер всех данных, записанных в ППЗУ субмодуля
	U08	bAdmNum;	// ADM-Interface number (номер ADM-интерфейса)
	U32	dSerialNum;	// serial number (серийный (физический) номер)
	U16	wType;		// ADM-submodule type (тип ADM-субмодуля)
	U08	bVersion;	// ADM-submodule version (версия ADM-субмодуля)
	U08	bDay;		// day of Data (день записи данных в ППЗУ субмодуля)
	U08	bMon;		// montag of Data (месяц записи данных в ППЗУ субмодуля)
	U16	wYear;		// year of Data (год записи данных в ППЗУ субмодуля)
} ICR_Id0080, *PICR_Id0080, ICR_IdAdm, *PICR_IdAdm;

// AMBPCI module configuration structure
// Конфигурационная структура базового модуля AMBPCI
typedef struct _ICR_Cfg4D50 {
	U16	wTag;		// tag of structure (AMBPCI_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bAdmIfCnt;	// ADM-Interface counter (количество ADM-интерфейсов)
} ICR_Cfg4D50, *PICR_Cfg4D50, ICR_CfgAmbpci, *PICR_CfgAmbpci;

// ADC configuration structure
// Конфигурационная структура АЦП
typedef struct _ICR_Cfg0100 {
	U16	wTag;		// tag of structure (ADC_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// number of ADC (номер АЦП)
	U08	bAdmNum;	// ADM-Interface number (номер интерфейса ADM)
	U08	bBits;		// Word Length (разрядность АЦП (число бит))
	U08	bEncoding;	// type of code (тип кодировки)
	U32	dMinRate;	// minimal sampling rate (минимальная частота дискретизации (Гц))
	U32	dMaxRate;	// maximal sampling rate (максимальная частота дискретизации (Гц))
	U16	wRange;		// input range (входной диапазон АЦП (мВольт))
} ICR_Cfg0100, *PICR_Cfg0100, ICR_CfgAdc, *PICR_CfgAdc;

// ADC FIFO configuration structure
// Конфигурационная структура FIFO АЦП
typedef struct _ICR_Cfg0200 {
	U16	wTag;		// tag of structure (ADC_FIFO_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the ADC FIFO (номер FIFO АЦП)
	U08	bAdmNum;	// ADM-Interface number (номер интерфейса ADM)
	U08	bDepth;		// Depth of FIFO (глубина (размер) FIFO (слова))
	U08	bBitsWidth;	// Width of FIFO (ширина FIFO (число бит в слове))
} ICR_Cfg0200, *PICR_Cfg0200, ICR_CfgAdcFifo, *PICR_CfgAdcFifo;

// DAC configuration structure
// Конфигурационная структура ЦАП
typedef struct _ICR_Cfg0300 {
	U16	wTag;		// tag of structure (DAC_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the DAC (номер ЦАП)
	U08	bAdmNum;	// ADM-Interface number (номер интерфейса ADM)
	U08	bBits;		// Word Length (разрядность ЦАП (число бит))
	U08	bEncoding;	// type of code (тип кодировки)
	U32	dMinRate;	// minimal sampling rate (минимальная частота дискретизации (Гц))
	U32	dMaxRate;	// maximal sampling rate (максимальная частота дискретизации (Гц))
	U16	wAFRange;	// DAC range for active filter (диапазон ЦАП при работе активного фильтра (мВольты))
	U16	wPFRange;	// DAC range for passive filter (диапазон ЦАП при работе пассивного фильтра (мВольты))
	U16	wAFCutoff;	// cufoff frequence of active filter (частота среза активного фильтра (сотни Гц))
	U16	wPFCutoffLo;// cutoff lowest frequence of passive filter (нижняя частота среза пассивного фильтра (кГц))
	U16	wPFCutoffHi;// cutoff highest frequence of passive filter (верхняя частота среза пассивного фильтра (кГц))
} ICR_Cfg0300, *PICR_Cfg0300, ICR_CfgDac, *PICR_CfgDac;

// Submodule DAC configuration structure
// Конфигурационная структура ЦАП субмодуля
typedef struct _ICR_Cfg0310 {
	U16	wTag;		// tag of structure (DAC_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the DAC (номер ЦАП)
	U08	bAdmNum;	// ADM-Interface number (номер интерфейса ADM)
	U08	bBits;		// Word Length (разрядность ЦАП (число бит))
	U08	bEncoding;	// type of code (тип кодировки)
	U32	dMinRate;	// minimal sampling rate (минимальная частота дискретизации (Гц))
	U32	dMaxRate;	// maximal sampling rate (максимальная частота дискретизации (Гц))
	U16	wRange;		// DAC range for active filter (диапазон ЦАП (мВольты))
} ICR_Cfg0310, *PICR_Cfg0310, ICR_CfgAdmDac, *PICR_CfgAdmDac;

// DAC FIFO configuration structure
// Конфигурационная структура FIFO ЦАП
typedef struct _ICR_Cfg0400 {
	U16	wTag;		// tag of structure (DAC_FIFO_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the DAC FIFO (номер FIFO ЦАП)
	U08	bAdmNum;	// ADM-Interface number (номер интерфейса ADM)
	U08	bDepth;		// Depth of FIFO (глубина (размер) FIFO (слова))
	U08	bBitsWidth;	// Width of FIFO (ширина FIFO (число бит в слове))
	U08	bIsCycle;	// Is cycle mode (возможность циклического режима работы FIFO)
} ICR_Cfg0400, *PICR_Cfg0400, ICR_CfgDacFifo, *PICR_CfgDacFifo;

// ADM PLD configuration structure
// Конфигурационная структура ПЛИС интерфейса ADM
typedef struct _ICR_Cfg0500 {
	U16	wTag;		// tag of structure (PLD_CFG_TAG)
	U16	wSize;		// size of bytes (размер всех следующих полей структуры)
	U08	bNumber;	// id number of the PLD (номер ПЛИС)
	U08	bAdmNum;	// ADM-Interface number (номер интерфейса ADM)
	U08	bType;		// type of PLD (0-EP1K,1-EP1KA,2-EP10KE,...) (серия(тип) ПЛИС)
	U16	wVolume;	// volume of PLD (объем ПЛИС)
	U16	wPins;		// pins counter of PLD (число выводов)
	U08	bSpeedGrade;// быстродействие 1,2,3,...
} ICR_Cfg0500, *PICR_Cfg0500, ICR_CfgAdmPld, *PICR_CfgAdmPld;

// Host PLD configuration structure
// Конфигурационная структура ПЛИС интерфейса хоста
typedef struct _ICR_Cfg0510 {
	U16	wTag;		// tag of structure (PLD_CFG_TAG)
	U16	wSize;		// size of bytes (размер всех следующих полей структуры)
	U08	bNumber;	// id number of the PLD (номер ПЛИС)
	U08	bType;		// type of Host PLD (TYPE_HostPLD)
	U16	wVolume;	// volume of Host PLD 
	U16	wPins;		// pins counter of Host PLD 
	U08	bSpeedGrade;// быстродействие (6 or 7)
} ICR_Cfg0510, *PICR_Cfg0510, ICR_CfgHostPld, *PICR_CfgHostPld;

// ADM interface configuration structure
// Конфигурационная структура интерфейса ADM, расположенного на базовом модуле
typedef struct _ICR_Cfg0600 {
	U16	wTag;		// tag of structure (ADMIF_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// ADM-Interface number (номер интерфейса ADM)
	U08	bAdcFifoCnt;// ADC FIFO counter (количество FIFO АЦП)
	U08	bDacCnt;	// DAC counter (количество ЦАП)
	U08	bDacFifoCnt;// DAC FIFO counter (количество FIFO ЦАП)
	U32	dRefGen[2];	// frequency of generators (значения опорных генераторов (Гц))
	U08	bIsStartSync;// Is start syncronization (наличие узла стартовой синхронизации: 1 - есть, 0 - нет)
	U08	bPldCnt;	// PLD counter (количество ПЛИС)
	U16	wRefPVS;	// Basic Voltage (опорное напряжение источников программируемых напряжений (мВольт))
	U08	bIsPio;		// Is digital port (PIO) (наличие порта ввода/вывода: 1 - есть, 0 - нет)
} ICR_Cfg0600, *PICR_Cfg0600, ICR_CfgAdmIf, *PICR_CfgAdmIf;

// ADM2 interface configuration structure
// Конфигурационная структура интерфейса ADM2, расположенного на базовом модуле
typedef struct _ICR_Cfg0610 {
	U16	wTag;		// tag of structure (ADMIF_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// ADM-Interface number (номер интерфейса ADM)
	U08	bAdcFifoCnt;// ADC FIFO counter (количество FIFO АЦП)
	U08	bDacCnt;	// DAC counter (количество ЦАП)
	U08	bDacFifoCnt;// DAC FIFO counter (количество FIFO ЦАП)
	U32	dRefGen[2];	// frequency of generators (значения опорных генераторов (Гц))
	U08	bIsStartSync;// Is start syncronization (наличие узла стартовой синхронизации: 1 - есть, 0 - нет)
	U08	bPldCnt;	// PLD counter (количество ПЛИС)
	U16	wRefPVS;	// Basic Voltage (опорное напряжение источников программируемых напряжений (мВольт))
	U08	bPioType;	// type of PIO (0-non, 1-TTL, 2-LVDS)
	U08	bType;		// ADM2 interface type (0-TTL, 1-LVDS)
} ICR_Cfg0610, *PICR_Cfg0610, ICR_CfgAdm2If, *PICR_CfgAdm2If;

// SDRAM configuration
// Конфигурационные параметры динамической памяти
typedef struct _ICR_Cfg0700 {
	USHORT	wTag;			// тэг структуры (SDRAM_CFG_TAG)
	USHORT	wSize;			// размер всех следующих полей структуры
	UCHAR	bNumber;		// номер структуры
	UCHAR	bSlotCnt;		// количество установленных слотов
	UCHAR	bModuleCnt;		// количество установленных DIMM-модулей
	UCHAR	bRowAddrBits;	// количество разрядов адреса строк
	UCHAR	bColAddrBits;	// количество разрядов адреса столбцов
	UCHAR	bModuleBanks;	// количество банков в DIMM-модуле
	UCHAR	bChipBanks;		// количество банков в микросхемах DIMM-модуля
} ICR_Cfg0700, *PICR_Cfg0700, ICR_CfgSdram, *PICR_CfgSdram;

#pragma pack(pop)    

#endif // __ICR_H_

//***************** End of file icr.h *********************
