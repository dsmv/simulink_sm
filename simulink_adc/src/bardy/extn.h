/***************************************************
*
* EXTN.H
*
* BRD_extension() cmd codes and data types
*
* (C) Instr.Sys. by Ekkore Apr, 1997-2011
*
* 15-11-2011 - added cmd PLDINFO and FMCPOWER (Dorokhin A.)
*
****************************************************/


#ifndef	__EXTN_H_
#define	__EXTN_H_


#include	"utypes.h"

//
//=== BRD_extension() cmd codes
//

enum
{
	BRDextn_BASE			= 0x100,	// Base addres of code range
	BRDextn_GET_MINPERBYTE	= 0x101,	// Get sizeof(char) in bytes
	BRDextn_TURN_DEASY		= 0x102,	// Turn Deasy Server On/Off
	BRDextn_GET_PLDINFO		= 0x104, 	// Get Pld info
	BRDextn_SET_FMCPOWER	= 0x105, 	// Set FMC-submodule power
	BRDextn_GET_FMCPOWER	= 0x106, 	// Get FMC-submodule power
	BRDextn_WRITE_FMCEXT	= 0x107, 	// Write to FMC Extender
	BRDextn_READ_FMCEXT		= 0x108, 	// Read from FMC Extender
	BRDextn_SET_IRQMINTIME	= 0x109, 	// Set min time between interrupt request 
	BRDextn_GET_IRQMINTIME	= 0x110, 	// Get min time between interrupt request 
};

//
//=== BRD_extension() data types
//

typedef struct
{
	U32	val;					// sizeof(char) in bytes
} BRDextn_GetMinperbyte;

typedef struct
{
	U32	isTurnOn;				// 0 - Turn Off, 1 - Turn On
	U32	timeout;				// milliseconds or BRDtim_FOREVER (if Turn Off)
	U32	isConsole;				// Create New Console
} BRDextn_TurnDeasy;
 
typedef struct _BRDextn_PLDINFO
{
	U32	pldId;			// Programmable Unit ID
	U32	version;		// Pld version / версия
	U32	modification;	// Pld modification / модификация
	U32	build;			// Pld build / сборка
} BRDextn_PLDINFO, *PBRDextn_PLDINFO;

typedef struct _BRDextn_FMCPOWER
{
	U32	onOff;		// 1 - on, 0 - off
	U32	value;		// voltage
	U32	slot;		// slot number (submodule number)
} BRDextn_FMCPOWER, *PBRDextn_FMCPOWER;

typedef struct _BRDextn_FMCEXT
{
	U32	dev;		// device address
	U32	addr;		// register address
	U32	value;		// value
} BRDextn_FMCEXT, *PBRDextn_FMCEXT;

#endif	// __EXTN_H_ 

//
//  End of File
//


