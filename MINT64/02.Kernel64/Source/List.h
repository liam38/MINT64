#ifndef __LIST_H__
#define __LIST_H__

#include "Types.h"

// 구조체
#pragma pack(push, 1)

// 데이터를 연결하는 자료구조
// 반드시 데이터의 가장 앞부분에 위치시킬 것. (해당 데이터의 위치 계산 편의를 위함)
typedef struct kListLinkStruct {
	// 다음 데이터의 addr과 data를 구분하기 위한 ID
	void* pvNext;
	QWORD qwID;
}LISTLINK;

/*
 // 리스트에 사용할 데이터를 정의하는 예
 // 반드시 가장 앞부분을 LISTLINK로 시작할 것.
 struct kListItemExampleStruct{
 	 LISTLINK stLink;

 	 int iData1;
 	 char cData2;
 }
 */

// 리스트를 관리하는 자료구조
typedef struct kListManagerStruct {
	// 리스트의 데이터 수
	int iItemCount;

	// 리스트의 첫 번째와 마지막 데이터의 어드레스
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
