/*
    Multimedia Extensions Sync Stream Manager Associate Control Block include file.
*/

#define MAX_PORT_NAME 40
#define MAX_PORTS 16

#define ACBTYPE_MMIO 1
#define ACBTYPE_MEM_PLAYL 3
#define ACBTYPE_CDDA 4
#define ACBTYPE_MISH 5
#define ACBTYPE_SET 6
#define ACBTYPE_NULLSH 7
#define ACBTYPE_MTSH 8
#define ACBTYPE_CODECSH 9

#define MIDI_MAP_ON 0
#define MIDI_MAP_OFF 1

#define TRACK_ACTIVATE 0
#define TRACK_DEACTIVATE 1

#pragma pack(1)

typedef struct _ACB_CDDA {
    ULONG ulACBLen;
    ULONG ulObjType;
    CHAR  bCDDrive;
} ACB_CDDA, FAR *PACB_CDDA;

#pragma pack()

#pragma pack(4)

typedef struct _ACB {
    ULONG ulACBLen;
    ULONG ulObjType;
    ULONG ulParm1;
    ULONG ulParm2;
} ACB, FAR *PACB;

typedef struct _ACB_MMIO {
    ULONG ulACBLen;
    ULONG ulObjType;
    HMMIO hmmio;
} ACB_MMIO, FAR *PACB_MMIO;

typedef struct _ACB_MEM_PLAYL {
    ULONG ulACBLen;
    ULONG ulObjType;
    PVOID pMemoryAddr;
} ACB_MEM_PLAYL, FAR *PACB_MEM_PLAYL;

typedef struct _ACB_MISH {
    ULONG   ulACBLen;
    ULONG   ulObjType;
    HSTREAM hstreamDefault;
    ULONG   ulDeviceTypeID;
    ULONG   ulpMapperPorts;
    ULONG   ulNumInStreams;
    HSTREAM hstreamIn[MAX_PORTS];
    ULONG   ulNumOutStreams;
    HSTREAM hstreamOut[MAX_PORTS];
} ACB_MISH, FAR *PACB_MISH;

typedef struct _ACB_SET {
    ULONG ulACBLen;
    ULONG ulObjType;
    ULONG ulFlags;
    ULONG ulReserved;
} ACB_SET, FAR *PACB_SET;

typedef struct _ACB_NULLSH {
    ULONG ulACBLen;
    ULONG ulObjType;
    PFN   pfnEntry;
    ULONG ulReserved;
} ACB_NULLSH, FAR *PACB_NULLSH;

typedef struct _ACB_MTSH {
    ULONG       ulACBLen;
    ULONG       ulObjType;
    HMMIO       hmmio;
    MMTRACKINFO mmtrackInfo;
    ULONG       ulFlags;
    ULONG       ulMaxBytesPerSec;
    ULONG       ulAvgBytesPerSec;
    MMTIME      mmtimePerFrame;
    ULONG       ulTotalFrames;
} ACB_MTSH, FAR *PACB_MTSH;

typedef struct _ACB_CODECSH {
    ULONG   ulACBLen;
    ULONG   ulObjType;
    HSTREAM hstreamToPair;
    PVOID   pMmioInfo;
    ULONG   ulInfoLength;
    PVOID   pCodecControl;
    ULONG   ulControlLength;
} ACB_CODECSH, FAR *PACB_CODECSH;

#pragma pack()

