#ifndef __SYNCHRONIZATION_H__
#define __SYNCHRONIZATION_H__

#include "Types.h"

// 구조체
// 1바이트로 정렬
#pragma pack(push, 1)

// Mutex
typedef struct kMutexStruct {
	// Task ID , Lock Count
	volatile QWORD qwTaskID;
	volatile DWORD dwLockCount;

	// 잠금 플래그
	volatile BOOL bLockFlag;

	// 8byte 크기를 맞추기 위한 Padding
	BYTE vbPadding[3];
}MUTEX;

#pragma pack(pop)

// Function
BOOL kLockForSystemData(void);
void kUnlockForSystemData(BOOL bInterruptFlag);

void kInitializeMutex(MUTEX* pstMutex);
void kLock(MUTEX* pstMutex);
void kUnlock(MUTEX* pstMutex);

#endif /*__SYNCHRONIZATION_H__*/
