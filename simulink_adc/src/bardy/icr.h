//****************** File icr.h ***************************
//
//  ����������� �������� � �������� 
//	��� ������ � ���������������� ����
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

const U16 END_TAG			= 0x0000; // tag of end data (���, ������������� �� ��������� ������)
const U16 ALT_END_TAG		= 0xffff; // alt tag of end data (�������������� ���, ������������� �� ��������� ������)

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

const U16 SDRAM_CFG_TAG		= 0x0700; // ��� ��� ��������� ���������������� ���������� ������������ ������

const U16 SUBMOD_CFGMEM_SIZE = 512; // ������ ����, ��������������� �� ���������

// Base module identification
// ����������������� ��������� �������� ������
typedef struct _ICR_Id4953 {
	U16	wTag;		// tag of structure (BASE_ID_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U16	wSizeAll;	// size of all data, writing into base module EPROM
	U32	dSerialNum;	// serial number (�������� (����������) �����)
	U16	wDeviceId;	// base module type (��� �������� ������)
	U08	bVersion;	// base module version (������ �������� ������)
	U08	bDay;		// day of Data (���� ������ ������ � ���� �������� ������)
	U08	bMon;		// montag of Data (����� ������ ������ � ���� �������� ������)
	U16	wYear;		// year of Data (��� ������ ������ � ���� �������� ������)
} ICR_Id4953, *PICR_Id4953, ICR_IdBase, *PICR_IdBase;

// ADM identification
// ����������������� ��������� ���������
typedef struct _ICR_Id0080 {
	U16	wTag;		// tag of structure (ADM_ID_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U16	wSizeAll;	// ������ ���� ������, ���������� � ���� ���������
	U08	bAdmNum;	// ADM-Interface number (����� ADM-����������)
	U32	dSerialNum;	// serial number (�������� (����������) �����)
	U16	wType;		// ADM-submodule type (��� ADM-���������)
	U08	bVersion;	// ADM-submodule version (������ ADM-���������)
	U08	bDay;		// day of Data (���� ������ ������ � ���� ���������)
	U08	bMon;		// montag of Data (����� ������ ������ � ���� ���������)
	U16	wYear;		// year of Data (��� ������ ������ � ���� ���������)
} ICR_Id0080, *PICR_Id0080, ICR_IdAdm, *PICR_IdAdm;

// AMBPCI module configuration structure
// ���������������� ��������� �������� ������ AMBPCI
typedef struct _ICR_Cfg4D50 {
	U16	wTag;		// tag of structure (AMBPCI_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bAdmIfCnt;	// ADM-Interface counter (���������� ADM-�����������)
} ICR_Cfg4D50, *PICR_Cfg4D50, ICR_CfgAmbpci, *PICR_CfgAmbpci;

// ADC configuration structure
// ���������������� ��������� ���
typedef struct _ICR_Cfg0100 {
	U16	wTag;		// tag of structure (ADC_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// number of ADC (����� ���)
	U08	bAdmNum;	// ADM-Interface number (����� ���������� ADM)
	U08	bBits;		// Word Length (����������� ��� (����� ���))
	U08	bEncoding;	// type of code (��� ���������)
	U32	dMinRate;	// minimal sampling rate (����������� ������� ������������� (��))
	U32	dMaxRate;	// maximal sampling rate (������������ ������� ������������� (��))
	U16	wRange;		// input range (������� �������� ��� (������))
} ICR_Cfg0100, *PICR_Cfg0100, ICR_CfgAdc, *PICR_CfgAdc;

// ADC FIFO configuration structure
// ���������������� ��������� FIFO ���
typedef struct _ICR_Cfg0200 {
	U16	wTag;		// tag of structure (ADC_FIFO_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the ADC FIFO (����� FIFO ���)
	U08	bAdmNum;	// ADM-Interface number (����� ���������� ADM)
	U08	bDepth;		// Depth of FIFO (������� (������) FIFO (�����))
	U08	bBitsWidth;	// Width of FIFO (������ FIFO (����� ��� � �����))
} ICR_Cfg0200, *PICR_Cfg0200, ICR_CfgAdcFifo, *PICR_CfgAdcFifo;

// DAC configuration structure
// ���������������� ��������� ���
typedef struct _ICR_Cfg0300 {
	U16	wTag;		// tag of structure (DAC_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the DAC (����� ���)
	U08	bAdmNum;	// ADM-Interface number (����� ���������� ADM)
	U08	bBits;		// Word Length (����������� ��� (����� ���))
	U08	bEncoding;	// type of code (��� ���������)
	U32	dMinRate;	// minimal sampling rate (����������� ������� ������������� (��))
	U32	dMaxRate;	// maximal sampling rate (������������ ������� ������������� (��))
	U16	wAFRange;	// DAC range for active filter (�������� ��� ��� ������ ��������� ������� (�������))
	U16	wPFRange;	// DAC range for passive filter (�������� ��� ��� ������ ���������� ������� (�������))
	U16	wAFCutoff;	// cufoff frequence of active filter (������� ����� ��������� ������� (����� ��))
	U16	wPFCutoffLo;// cutoff lowest frequence of passive filter (������ ������� ����� ���������� ������� (���))
	U16	wPFCutoffHi;// cutoff highest frequence of passive filter (������� ������� ����� ���������� ������� (���))
} ICR_Cfg0300, *PICR_Cfg0300, ICR_CfgDac, *PICR_CfgDac;

// Submodule DAC configuration structure
// ���������������� ��������� ��� ���������
typedef struct _ICR_Cfg0310 {
	U16	wTag;		// tag of structure (DAC_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the DAC (����� ���)
	U08	bAdmNum;	// ADM-Interface number (����� ���������� ADM)
	U08	bBits;		// Word Length (����������� ��� (����� ���))
	U08	bEncoding;	// type of code (��� ���������)
	U32	dMinRate;	// minimal sampling rate (����������� ������� ������������� (��))
	U32	dMaxRate;	// maximal sampling rate (������������ ������� ������������� (��))
	U16	wRange;		// DAC range for active filter (�������� ��� (�������))
} ICR_Cfg0310, *PICR_Cfg0310, ICR_CfgAdmDac, *PICR_CfgAdmDac;

// DAC FIFO configuration structure
// ���������������� ��������� FIFO ���
typedef struct _ICR_Cfg0400 {
	U16	wTag;		// tag of structure (DAC_FIFO_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// id number of the DAC FIFO (����� FIFO ���)
	U08	bAdmNum;	// ADM-Interface number (����� ���������� ADM)
	U08	bDepth;		// Depth of FIFO (������� (������) FIFO (�����))
	U08	bBitsWidth;	// Width of FIFO (������ FIFO (����� ��� � �����))
	U08	bIsCycle;	// Is cycle mode (����������� ������������ ������ ������ FIFO)
} ICR_Cfg0400, *PICR_Cfg0400, ICR_CfgDacFifo, *PICR_CfgDacFifo;

// ADM PLD configuration structure
// ���������������� ��������� ���� ���������� ADM
typedef struct _ICR_Cfg0500 {
	U16	wTag;		// tag of structure (PLD_CFG_TAG)
	U16	wSize;		// size of bytes (������ ���� ��������� ����� ���������)
	U08	bNumber;	// id number of the PLD (����� ����)
	U08	bAdmNum;	// ADM-Interface number (����� ���������� ADM)
	U08	bType;		// type of PLD (0-EP1K,1-EP1KA,2-EP10KE,...) (�����(���) ����)
	U16	wVolume;	// volume of PLD (����� ����)
	U16	wPins;		// pins counter of PLD (����� �������)
	U08	bSpeedGrade;// �������������� 1,2,3,...
} ICR_Cfg0500, *PICR_Cfg0500, ICR_CfgAdmPld, *PICR_CfgAdmPld;

// Host PLD configuration structure
// ���������������� ��������� ���� ���������� �����
typedef struct _ICR_Cfg0510 {
	U16	wTag;		// tag of structure (PLD_CFG_TAG)
	U16	wSize;		// size of bytes (������ ���� ��������� ����� ���������)
	U08	bNumber;	// id number of the PLD (����� ����)
	U08	bType;		// type of Host PLD (TYPE_HostPLD)
	U16	wVolume;	// volume of Host PLD 
	U16	wPins;		// pins counter of Host PLD 
	U08	bSpeedGrade;// �������������� (6 or 7)
} ICR_Cfg0510, *PICR_Cfg0510, ICR_CfgHostPld, *PICR_CfgHostPld;

// ADM interface configuration structure
// ���������������� ��������� ���������� ADM, �������������� �� ������� ������
typedef struct _ICR_Cfg0600 {
	U16	wTag;		// tag of structure (ADMIF_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// ADM-Interface number (����� ���������� ADM)
	U08	bAdcFifoCnt;// ADC FIFO counter (���������� FIFO ���)
	U08	bDacCnt;	// DAC counter (���������� ���)
	U08	bDacFifoCnt;// DAC FIFO counter (���������� FIFO ���)
	U32	dRefGen[2];	// frequency of generators (�������� ������� ����������� (��))
	U08	bIsStartSync;// Is start syncronization (������� ���� ��������� �������������: 1 - ����, 0 - ���)
	U08	bPldCnt;	// PLD counter (���������� ����)
	U16	wRefPVS;	// Basic Voltage (������� ���������� ���������� ��������������� ���������� (������))
	U08	bIsPio;		// Is digital port (PIO) (������� ����� �����/������: 1 - ����, 0 - ���)
} ICR_Cfg0600, *PICR_Cfg0600, ICR_CfgAdmIf, *PICR_CfgAdmIf;

// ADM2 interface configuration structure
// ���������������� ��������� ���������� ADM2, �������������� �� ������� ������
typedef struct _ICR_Cfg0610 {
	U16	wTag;		// tag of structure (ADMIF_CFG_TAG)
	U16	wSize;		// size of all following fields of structure (without wTag + wSize) = sizeof(ICR_IdBase) - 4
	U08	bNumber;	// ADM-Interface number (����� ���������� ADM)
	U08	bAdcFifoCnt;// ADC FIFO counter (���������� FIFO ���)
	U08	bDacCnt;	// DAC counter (���������� ���)
	U08	bDacFifoCnt;// DAC FIFO counter (���������� FIFO ���)
	U32	dRefGen[2];	// frequency of generators (�������� ������� ����������� (��))
	U08	bIsStartSync;// Is start syncronization (������� ���� ��������� �������������: 1 - ����, 0 - ���)
	U08	bPldCnt;	// PLD counter (���������� ����)
	U16	wRefPVS;	// Basic Voltage (������� ���������� ���������� ��������������� ���������� (������))
	U08	bPioType;	// type of PIO (0-non, 1-TTL, 2-LVDS)
	U08	bType;		// ADM2 interface type (0-TTL, 1-LVDS)
} ICR_Cfg0610, *PICR_Cfg0610, ICR_CfgAdm2If, *PICR_CfgAdm2If;

// SDRAM configuration
// ���������������� ��������� ������������ ������
typedef struct _ICR_Cfg0700 {
	USHORT	wTag;			// ��� ��������� (SDRAM_CFG_TAG)
	USHORT	wSize;			// ������ ���� ��������� ����� ���������
	UCHAR	bNumber;		// ����� ���������
	UCHAR	bSlotCnt;		// ���������� ������������� ������
	UCHAR	bModuleCnt;		// ���������� ������������� DIMM-�������
	UCHAR	bRowAddrBits;	// ���������� �������� ������ �����
	UCHAR	bColAddrBits;	// ���������� �������� ������ ��������
	UCHAR	bModuleBanks;	// ���������� ������ � DIMM-������
	UCHAR	bChipBanks;		// ���������� ������ � ����������� DIMM-������
} ICR_Cfg0700, *PICR_Cfg0700, ICR_CfgSdram, *PICR_CfgSdram;

#pragma pack(pop)    

#endif // __ICR_H_

//***************** End of file icr.h *********************
