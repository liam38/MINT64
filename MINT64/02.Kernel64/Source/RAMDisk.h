#ifndef __RAMDISK_H__
#define __RAMDISK_H__

#include "Types.h"
#include "Synchronization.h"
#include "HardDisk.h"

// macro
// �� ��ũ�� �� ���� ��. 8MB ����
#define RDD_TOTALSECTORCOUNT 		( 8 * 1024 * 1024 / 512 )

// Structure
// 1 byte ����
#pragma pack(push, 1)

// �� ��ũ�� �ڷᱸ���� �����ϴ� ����ü
typedef struct kRDDManagerStruct {
	// �� ��ũ������ �Ҵ���� �޸� ����
	BYTE* pbBuffer;

	// �� ���� ��
	DWORD dwTotalSectorCount;

	// ����ȭ ��ü
	MUTEX stMutex;
}RDDMANAGER;

#pragma pack(pop)

// Function
BOOL kInitializeRDD(DWORD dwTotalSectorCount);
BOOL kReadRDDInformation(BOOL bPrimary, BOOL bMaster, HDDINFORMATION* pstHDDInformation);
int kReadRDDSector(BOOL bPrimary, BOOL bMaster, DWORD dwLBA, int iSectorCount, char* pcBuffer);
int kWriteRDDSector(BOOL bPrimary, BOOL bMaster, DWORD dwLBA, int iSectorCount, char* pcBuffer);

#endif /*__RAMDISK_H__*/
