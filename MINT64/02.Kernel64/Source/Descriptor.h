#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "Types.h"

// Macro
//===================================================================
//GDT
//===================================================================
#define GDT_TYPE_CODE			0x0A
#define GDT_TYPE_DATA			0x02
#define GDT_TYPE_TSS			0x09
#define GDT_FLAGS_LOWER_S		0x10
#define GDT_FLAGS_LOWER_DPL0	0x00
#define GDT_FLAGS_LOWER_DPL1	0x20
#define GDT_FLAGS_LOWER_DPL2	0x40
#define GDT_FLAGS_LOWER_DPL3	0x60
#define GDT_FLAGS_LOWER_P		0x80
#define GDT_FLAGS_UPPER_L		0x20
#define GDT_FLAGS_UPPER_DB		0x40
#define GDT_FLAGS_UPPER_G		0x80

//실제 사용 macro
// Lower Flags는 Code/Data/TSS, DPL0, Present로 설정
#define GDT_FLAGS_LOWER_KERNELCODE ( GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_KERNELDATA ( GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_TSS ( GDT_FLAGS_LOWER_DPL0 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_USERCODE ( GDT_TYPE_CODE | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P )
#define GDT_FLAGS_LOWER_USERDATA ( GDT_TYPE_DATA | GDT_FLAGS_LOWER_S | GDT_FLAGS_LOWER_DPL3 | GDT_FLAGS_LOWER_P )

// Upper Flags는 Granulaty로 설정, 코드 및 데이터는 64bit 추가
#define GDT_FLAGS_UPPER_CODE ( GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L )
#define GDT_FLAGS_UPPER_DATA ( GDT_FLAGS_UPPER_G | GDT_FLAGS_UPPER_L )
#define GDT_FLAGS_UPPER_TSS ( GDT_FLAGS_UPPER_G )

// Segment Descriptor offset
#define GDT_KERNELCODESEGMENT 	0x08
#define GDT_KERNELDATASEGMENT 	0x10
#define GDT_TSSSEGMENT			0x18

// 기타 GDT macro
// GDTR 시작 addr. (1MB~264KB : page table area)
#define GDTR_STARTADDRESS 	0x142000
// 8byte entry의 개수, NULL/KERNEL CODE/KERNEL DATA
#define GDT_MAXENTRY8COUNT	3
// 16byte entry의 개수, TSS
#define GDT_MAXENTRY16COUNT 1
// GDT Table size
#define GDT_TABLESIZE		((sizeof(GDTENTRY8) * GDT_MAXENTRY8COUNT) + (sizeof(GDTENTRY16) * GDT_MAXENTRY16COUNT))
#define TSS_SEGMENTSIZE 	(sizeof(TSSSEGMENT))

//===========================================================================
// IDT
//===========================================================================
#define IDT_TYPE_INTERRUPT 		0x0E
#define IDT_TYPE_TRAP 			0x0F
#define IDT_FLAGS_DPL0 			0x00
#define IDT_FLAGS_DPL1			0x20
#define IDT_FLAGS_DPL2			0x40
#define IDT_FLAGS_DPL3			0x60
#define IDT_FLAGS_P				0x80
#define IDT_FLAGS_IST0			0
#define IDT_FLAGS_IST1			1

//실제 사용 macro
#define IDT_FLAGS_KERNEL 		( IDT_FLAGS_DPL0 | IDT_FLAGS_P )
#define IDT_FLAGS_USER			( IDT_FLAGS_DPL3 | IDT_FLAGS_P )

// 기타 IDT 관련 macro
// IDT Entry 개수
#define IDT_MAXENTRYCOUNT 		100
// IDTR 시작 addr. TSS segment 뒤에 위치
#define IDTR_STARTADDRESS 		( GDTR_STARTADDRESS + sizeof(GDTR) + GDT_TABLESIZE + TSS_SEGMENTSIZE )
// IDT Table 시작 addr.
#define IDT_STARTADDRESS 		( IDTR_STARTADDRESS + sizeof(IDTR) )
// IDT Table Size
#define IDT_TABLESIZE			( IDT_MAXENTRYCOUNT * sizeof(IDTENTRY) )

//IST 시작 addr, IST size
#define IST_STARTADDRESS 		0x700000
#define IST_SIZE				0x100000

// 구조체
// 1byte 정렬
#pragma pack( push, 1 )

//GDTR, IDTR 구조체
typedef struct kGDTRStruct {
	WORD wLimit;
	QWORD qwBaseAddress;
	//16byte addr 정렬을 위해 추가
	WORD wPading;
	DWORD dwPading;
}GDTR, IDTR;

//8byte 크기의 GDT entry 구조
typedef struct kGDTEntry8Struct {
	WORD wLowerLimit;
	WORD wLowerBaseAddress;
	BYTE bUpperBaseAddress1;
	//4byte Type, 1bit S, 2bit DPL, 1bit P
	BYTE bTypeAndLowerFlag;
	//4bit segment Limit, 1bbit AVL, L, D/B, G
	BYTE bUpperLimitAndUpperFlag;
	BYTE bUpperBaseAddress2;
}GDTENTRY8;

//16byte 크기의 GDT entry 구조
typedef struct kGDTEntry16Sturct {
	WORD wLowerLimit;
	WORD wLowerBaseAddress;
	BYTE bMiddleBaseAddress1;
	//4bit Type, 1bit 0, 2bit DPL, 1bit P
	BYTE bTypeAndLowerFlag;
	//4bit segment limit, 1bit AVL, 0, 0, G
	BYTE bUpperLimitAndUpperFlag;
	BYTE bMiddleBaseAddress2;
	DWORD dwUpperBaseAddress;
	DWORD dwReserved;
}GDTENTRY16;

//TSS Data 구조체
typedef struct kTSSDataStruct {
	DWORD dwReserved1;
	QWORD qwRsp[3];
	QWORD qwReserved2;
	QWORD qwIST[7];
	QWORD qwReserved3;
	WORD wReserved;
	WORD wIOMapBaseAddress;
}TSSSEGMENT;

//IDT gate descriptor 구조체
typedef struct kIDTEntryStruct {
	WORD wLowerBaseAddress;
	WORD wSegmentSelector;
	//3bit IST, 5bit 0
	BYTE bIST;
	//4bit Type, 1bit 0, 2bit DPL, 1bit P
	BYTE bTypeAndFlags;
	WORD wMiddleBaseAddress;
	DWORD dwUpperBaseAddress;
	DWORD dwReserved;
}IDTENTRY;

#pragma pack( pop )

//함수
void kInitializeGDTTableAndTSS(void);
void kSetGDTEntry8( GDTENTRY8* pstEntry, DWORD dwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType );
void kSetGDTEntry16( GDTENTRY16* pstEntry, QWORD qwBaseAddress, DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType );
void kInitializeTSSSegment( TSSSEGMENT* pstTSS );

void kInitializeIDTTables(void);
void kSetIDTEntry( IDTENTRY* pstEntry, void* pvHandler, WORD wSelector, BYTE bIST, BYTE bFlags, BYTE bType );
void kDummyHandler(void);

#endif /*__DESCIPROT_H__*/
