#ifndef __RAMDISK_H__
#define __RAMDISK_H__

#include "Types.h"
#include "Synchronization.h"
#include "HardDisk.h"

// macro
// 램 디스크의 총 섹터 수. 8MB 생성
#define RDD_TOTALSECTORCOUNT 		( 8 * 1024 * 1024 / 512 )

// Structure
// 1 byte 정렬
#pragma pack(push, 1)

// 램 디스크의 자료구조를 저장하는 구조체
typedef struct kRDDManagerStruct {
	// 램 디스크용으로 할당받은 메모리 영역
	BYTE* pbBuffer;

	// 총 섹터 수
	DWORD dwTotalSectorCount;

	// 동기화 객체
	MUTEX stMutex;
}RDDMANAGER;

#pragma pack(pop)

// Function
BOOL kInitializeRDD(DWORD dwTotalSectorCount);
BOOL kReadRDDInformation(BOOL bPrimary, BOOL bMaster, HDDINFORMATION* pstHDDInformation);
int kReadRDDSector(BOOL bPrimary, BOOL bMaster, DWORD dwLBA, int iSectorCount, char* pcBuffer);
int kWriteRDDSector(BOOL bPrimary, BOOL bMaster, DWORD dwLBA, int iSectorCount, char* pcBuffer);

#endif /*__RAMDISK_H__*/
