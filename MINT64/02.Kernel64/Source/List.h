#ifndef __LIST_H__
#define __LIST_H__

#include "Types.h"

// ����ü
#pragma pack(push, 1)

// �����͸� �����ϴ� �ڷᱸ��
// �ݵ�� �������� ���� �պκп� ��ġ��ų ��. (�ش� �������� ��ġ ��� ���Ǹ� ����)
typedef struct kListLinkStruct {
	// ���� �������� addr�� data�� �����ϱ� ���� ID
	void* pvNext;
	QWORD qwID;
}LISTLINK;

/*
 // ����Ʈ�� ����� �����͸� �����ϴ� ��
 // �ݵ�� ���� �պκ��� LISTLINK�� ������ ��.
 struct kListItemExampleStruct{
 	 LISTLINK stLink;

 	 int iData1;
 	 char cData2;
 }
 */

// ����Ʈ�� �����ϴ� �ڷᱸ��
typedef struct kListManagerStruct {
	// ����Ʈ�� ������ ��
	int iItemCount;

	// ����Ʈ�� ù ��°�� ������ �������� ��巹��
	void* pvHeader;
	void* pvTail;
}LIST;

#pragma pack(pop)

// Function
void kInitializeList(LIST* pstList);
int kGetListCount(const LIST* pstList);
void kAddListToTail(LIST* pstList, void* pvItem);
void kAddListToHeader(LIST* pstList, void* pvItem);
void* kRemoveList(LIST* pstList, QWORD qwID);
void* kRemoveListFromHeader(LIST* pstList);
void* kRemoveListFromTail(LIST* pstList);
void* kFindList(const LIST* pstList, QWORD qwID);
void* kGetHeaderFromList(const LIST* pstList);
void* kGetTailFromList(const LIST* pstList);
void* kGetNextFromList(const LIST* pstList, void* pstCurrent);

#endif /*__LIST_H__*/