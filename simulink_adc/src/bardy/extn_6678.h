/***************************************************
*
* EXTN_6678.H
*
* BRD_extension() cmd codes and data types
*
* (C) InSys by Dorokhin Nov, 2012
*
****************************************************/


#ifndef	__EXTN_6678_H_
#define	__EXTN_6678_H_


#include	"utypes.h"

//
//=== BRD_extension() cmd codes
//

enum
{
	//BRDextn_BASE_DO	= 0x6800,		// Base addres of code range
	//BRDextn_DMA_READ_START,			// start DMA for read memory from TI6678 to Host
	//BRDextn_DMA_WRITE_START,		// start DMA for write memory from Host to TI6678
	//BRDextn_DMA_READ_STATE,			// get DMA read state
	//BRDextn_DMA_WRITE_STATE,		// get DMA write state
	//BRDextn_DMA_ALLOC_BUF,			// buffer allocate for DMA transfer
	//BRDextn_DMA_FREE_BUF,			// buffer free (for DMA_ALLOC_BUF)
	//BRDextn_IRQ_STATE				// get interrupt PCIe state
	BRDextn_DMA_START_ADDR,			// start DMA address
	BRDextn_SOFT_RESET,				// start DMA address
};

#pragma pack(push, 1) 

typedef struct {
	U32	padr_ti;			// IN - physical address of TI6678
	U32	*vadr;				// OUT - virtual address of process
	U64	physadr;			// OUT - physical address on bus
	U32	maxsize;			// OUT - size of buffer
	U64 phyInt;				// OUT - TI6678 interrupt generate physical address
	U32	intFpgaCode;		// OUT - interrupt code from FPGA
} BRDextn_dmaStartAddr;

typedef struct {
	U32	timeout;			// timeout after reset
} BRDextn_softReset;

//typedef struct {
//	U32	chan;				// chanel number
//	U32	pciAdr;				// PCI physical address
//	U32	locAdr;				// TI6678 local physical address
//	U32	size;				// size of bytes
//	U32	flag;				// flag
//	U32	timeout;			// timeout
//} BRDextn_dmaStart;
//
//typedef struct {
//	U32	chan;				// chanel number
//	U32	state;				// state
//	U32	timeout;			// timeout
//} BRDextn_dmaState;
//
//typedef struct {
//	U64	padr;				// physical address
//	U32	*vadr;				// virtual addrev
//	U32	size;				// size of buffer
//} BRDextn_dmaAllocBuf;
//
//typedef struct {
//	U32	*vadr;				// virtual addrev
//} BRDextn_dmaFreeBuf;

#pragma pack(pop)    

#endif	// __EXTN_DO_H_ 

//
//  End of File
//


