
#ifndef __ISDLL_H
 #define __ISDLL_H

#ifdef ISDLL_EXPORTS
 #define ISDLL_API __declspec(dllexport)
#else
 #define ISDLL_API __declspec(dllimport)
#endif // ISDLL_EXPORTS

// Transfer command
typedef enum 
{
        // single I/O transfer command
        TCMD_RP_BYTE  = 10,     // read byte from I/O port
        TCMD_RP_WORD  = 11,     // read word from I/O port
        TCMD_RP_DWORD = 12,     // read dword from I/O port
        TCMD_WP_BYTE  = 13,     // write byte to I/O port
        TCMD_WP_WORD  = 14,     // write word to I/O port
        TCMD_WP_DWORD = 15,     // write dword to I/O port

        // block I/O  transfer command
        TCMD_RP_BYTES  = 20,    // read bytes from I/O port
        TCMD_RP_WORDS  = 21,    // read words from I/O port
        TCMD_RP_DWORDS = 22,    // read dwords from I/O port
        TCMD_WP_BYTES  = 23,    // write bytes to I/O port
        TCMD_WP_WORDS  = 24,    // write words to I/O port
        TCMD_WP_DWORDS = 25,    // write dwords to I/O port
        
        // single memory transfer command
        TCMD_RM_BYTE  = 30,     // read byte from memory
        TCMD_RM_WORD  = 31,     // read word from memory
        TCMD_RM_DWORD = 32,     // read dword from memory
        TCMD_WM_BYTE  = 33,     // write byte to memory
        TCMD_WM_WORD  = 34,     // write word to memory
        TCMD_WM_DWORD = 35,     // write dword to memory
        TCMD_RM_QWORD = 36,     // read qword to memory
        TCMD_WM_QWORD = 37,     // write qword to memory

        // block memory transfer command
        TCMD_RM_BYTES  = 40,    // read bytes from memory
        TCMD_RM_WORDS  = 41,    // read words from memory
        TCMD_RM_DWORDS = 42,    // read dwords from memory
        TCMD_WM_BYTES  = 43,    // write bytes to memory
        TCMD_WM_WORDS  = 44,    // write words to memory
        TCMD_WM_DWORDS = 45,    // write dwords to memory
        TCMD_RM_QWORDS = 46,    // read qwords to memory
        TCMD_WM_QWORDS = 47,    // write qwords to memory

} TRANSFER_CMD;

#pragma pack(push,1)

// Transfer Data Control Structure
typedef struct _TRANSFER_DATA 
{
        ULONG   cmd;            // command
        ULONG64  addr;        // i/o port or memory address
        union 
        {
            UCHAR		b;              // value for write ( return value for read )
            USHORT		w;              // value for write ( return value for read )
            ULONG		dw;             // value for write ( return value for read )
            ULONG64		qw;             // value for write ( return value for read )
            void		*addr;          // user address for string transfer
        } data;
        ULONG   size;           // size of bytes for string transfer
        ULONG   inc;            // address increment for string transfer
        LONG    opt;            // options
} TRANSFER_DATA, *PTRANSFER_DATA;

typedef union PciDeviceFxn { /* PCI Device and Function Numbers */
  ULONG AsWhole;
//  UCHAR AsWhole;
  struct {
    USHORT fno : 3, /* Function Number */
		   dno : 5; /* Device Number ( Slot) */
  } ByBits;
} PCI_DEVFXN;

typedef struct PciDevice { /* PCI Device */
  PCI_DEVFXN tar;
  UCHAR      bus;		// Bus number 
} PCI_DEVICE;

typedef struct PciClassCode {
  UCHAR intf; /* Programming Interface Class Code */
  UCHAR sub;  /* Secondary Sub-Class Code */
  UCHAR base; /* Base Class Code */
} PCI_CLASS;

/*
 * PCI Configuration Space Header Region
 */
typedef struct PciCfgSpaceHdrRgn {
  USHORT      VID;    /* Vendor Identification Register */
  USHORT      DID;    /* Device Identification Register */
  USHORT      PCICMD; /* PCI Command Register */
  USHORT      PCISTS; /* PCI Status Register */
  UCHAR      RID;    /* Revision Identification Register */
  PCI_CLASS CLCD;   /* Class Code */
  UCHAR      CALN;   /* Cache Line Size Register */
  UCHAR      LAT;    /* Master Latency Timer */
  UCHAR      HDR;    /* Header Type */
  UCHAR      BIST;   /* Built-in Self-test */
  union PciHeaderTypeRgn {
    struct PciHeaderTypeOne {
      ULONG BADR[6]; /* Base Address Registers */
      ULONG CISP;    /* CardBus Card-Information-Structure Pointer */
      USHORT  SID;     /* Subsystem ID */
      USHORT  SVID;    /* Subsystem Vendor ID */
      ULONG EXROM;   /* Expansion ROM Base Address */
      ULONG RESV[2]; /* Reserved */
      UCHAR  INTLN;   /* Interrupt Line */
      UCHAR  INTPIN;  /* Interrupt Pin */
      UCHAR  MINGNT;  /* Minimum Grant */
      UCHAR  MAXLAT;  /* Maximum Latency */
    } Type00h;
    struct PciHeaderTypeTwo {
      ULONG BADR[2]; /* Base Address Registers */
      UCHAR  PRINUM;  /* Primary Bus Number */
      UCHAR  SECNUM;  /* Secondary Bus Number */
      UCHAR  SUBNUM;  /* Subordinate Bus Number */
      UCHAR  SECLAT;  /* Secondary Latency Timer */
      UCHAR  IOBASE;  /* I/O Base */
      UCHAR  IOLIM;   /* I/O Limit */
      USHORT  SECSTS;  /* Secondary Status */
      USHORT  MEMBASE; /* Memory Base */
      USHORT  MEMLIM;  /* Memory Limit */
      USHORT  PFMBASE; /* Prefetchable Memory Base */
      USHORT  PFMLIM;  /* Prefetchable Memory Limit */
      ULONG PBU32B;  /* Prefetchable Base Upper 32 Bits */
      ULONG PLU32B;  /* Prefetchable Limit Upper 32 Bits */
      USHORT  IOBU16B; /* I/O Base Upper 16 Bits */
      USHORT  IOLU16B; /* I/O Limit Upper 16 Bits */
      ULONG RESV;    /* Reserved */
      ULONG EXROM;   /* Expansion ROM Base Address */
      UCHAR  INTLN;   /* Interrupt Line */
      UCHAR  INTPIN;  /* Interrupt Pin */
      USHORT  BRGCTRL; /* Bridge Control */
    } Type01h;
  } Hdr;

  UCHAR	vendorSpecific[192];

} PCI_HEADER;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

// API Functions

ISDLL_API HANDLE __stdcall UniDrv_Open(int deviceID);
ISDLL_API int __stdcall UniDrv_Close(HANDLE handle);
ISDLL_API int __stdcall UniDrv_WinVer(HANDLE handle);
ISDLL_API int __stdcall UniDrv_Transfer(HANDLE handle, TRANSFER_DATA *trans);
ISDLL_API int __stdcall UniDrv_Transfer_Multi(HANDLE handle, TRANSFER_DATA *trans, ULONG cnt);

ISDLL_API int __stdcall UniDrv_PCI_FindDevice(HANDLE handle, USHORT DeviceID, USHORT VendorID, USHORT Cnt, PCI_DEVICE *dev);
ISDLL_API int __stdcall UniDrv_PCI_ReadCfgSpace(HANDLE handle, PCI_DEVICE *dev, PCI_HEADER *pciCfg);
ISDLL_API int __stdcall UniDrv_PCI_WriteCfgSpace(HANDLE handle, PCI_DEVICE *dev, PCI_HEADER *pciCfg);
ISDLL_API int __stdcall UniDrv_PCI_ReadCfgData(HANDLE handle, PCI_DEVICE *dev, USHORT offs, USHORT SizeOfBytes, void *data);
ISDLL_API int __stdcall UniDrv_PCI_WriteCfgData(HANDLE handle, PCI_DEVICE *dev, USHORT offs, USHORT SizeOfBytes, void *data);

ISDLL_API int __stdcall UniDrv_Mem_Map(HANDLE handle, ULONG  bytes, ULONG64 phAddr, void *linAddr);
ISDLL_API int __stdcall UniDrv_Mem_UnMap(HANDLE handle, void *linAddr);
ISDLL_API int __stdcall UniDrv_Mem_Alloc(HANDLE handle, ULONG bytes, ULONG64 *phAddr, void *linAddr, ULONG memLimit = 1);
ISDLL_API int __stdcall UniDrv_Mem_Free(HANDLE handle, void *linAddr);
ISDLL_API int __stdcall UniDrv_Mem_Free_All(HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif   // __ISDLL_H
