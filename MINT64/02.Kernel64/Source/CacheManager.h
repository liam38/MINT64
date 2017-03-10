#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include "Types.h"

// macro
// Ŭ������ ��ũ ���̺� ������ �ִ� Ű�� ������ ����
#define CACHE_MAXCLUSTERLINKTABLEAREACOUNT 		16
// ������ ������ �ִ� ĳ�� ������ ��
#define CACHE_MAXDATAAREACOUNT 					32
// ��ȿ���� ���� �±�, ��� �ִ� ĳ�� ����
#define CACHE_INVALIDTAG 						0xFFFFFFFF

// ĳ�� ���̺��� �ִ� ����. Ŭ������ ��ũ ���̺�� ������ ������ �����Ƿ� 2�� ����
#define CACHE_MAXCACHETABLEINDEX 				2
// Ŭ������ ��ũ ���̺� ������ �ε���
#define CACHE_CLUSTERLINKTABLEAREA 				0
// ������ ������ �ε���
#define CACHE_DATAAREA 							1

// Structure
// ���� �ý��� ĳ�ø� �����ϴ� ĳ�� ������ ����ü
typedef struct kCacheBufferStruct {
	// ĳ�ÿ� �����ϴ� Ŭ������ ��ũ ���̺� �����̳� ������ ������ �ε���
	DWORD dwTag;

	// ĳ�� ���ۿ� ������ �ð�
	DWORD dwAccessTime;

	// �������� ���� ���� ����
	BOOL bChanged;

	// ������ ����
	BYTE* pbBuffer;
}CACHEBUFFER;

// ���� �ý��� ĳ�ø� �����ϴ� ĳ�� �Ŵ����� ����ü
typedef struct kCacheManagerStruct {
	// Ŭ������ ��ũ ���̺� ������ ������ ������ ���� �ð� �ʵ�
	DWORD vdwAccessTime[CACHE_MAXCACHETABLEINDEX];

	// Ŭ������ ��ũ ���̺� ������ ������ ������ ������ ����
	BYTE* vpbBuffer[CACHE_MAXCACHETABLEINDEX];

	// Ŭ������ ��ũ ���̺� ������ ������ ������ ĳ�� ����
	// �� �� �߿��� ū ����ŭ ����
	CACHEBUFFER vvstCacheBuffer[CACHE_MAXCACHETABLEINDEX][CACHE_MAXDATAAREACOUNT];

	// ĳ�� ������ �ִ��� ����
	DWORD vdwMaxCount[CACHE_MAXCACHETABLEINDEX];
}CACHEMANAGER;

// Function
BOOL kInitializeCacheManager(void);
CACHEBUFFER* kAllocateCacheBuffer(int iCacheTableIndex);
CACHEBUFFER* kFindCacheBuffer(int iCacheTableIndex, DWORD dwTag);
CACHEBUFFER* kGetVictimInCacheBuffer(int iCacheTableIndex);
void kDiscardAllCacheBuffer(int iCacheTableIndex);
BOOL kGetCacheBufferAndCount(int iCacheTableIndex, CACHEBUFFER** ppstCacheBuffer, int* piMaxCount);

static void kCutDownAccessTime(int iCacheTableIndex);

#endif /*__CACHEMANAGER_H__*/
