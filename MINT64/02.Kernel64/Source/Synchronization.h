#ifndef __SYNCHRONIZATION_H__
#define __SYNCHRONIZATION_H__

#include "Types.h"

// ����ü
// 1����Ʈ�� ����
#pragma pack(push, 1)

// Mutex
typedef struct kMutexStruct {
	// Task ID , Lock Count
	volatile QWORD qwTaskID;
	volatile DWORD dwLockCount;

	// ��� �÷���
	volatile BOOL bLockFlag;

	// 8byte ũ�⸦ ���߱� ���� Padding
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
