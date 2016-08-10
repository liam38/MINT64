#include "Utility.h"
#include "AssemblyUtility.h"

//메모리를 특정 값으로 채움
void kMemSet(void* pvDestination, BYTE bData, int iSize) {
	int index;

	for(index = 0; index < iSize; index++)
		((char*)pvDestination)[index] = bData;
}

//메모리 복사
int kMemCpy(void* pvDestination, const void* pvSource, int iSize) {
	int index;

	for(index = 0; index < iSize; index++)
		((char*)pvDestination)[index] = ((char*)pvSource)[index];

	return iSize;
}

//메모리 비교
int kMemCmp(const void* pvDestination, const void* pvSource, int iSize) {
	int index;
	char cTemp;

	for (index = 0; index < iSize; index++) {
		cTemp = ((char*)pvDestination)[index] - ((char*)pvSource)[index];
		if( cTemp != 0 )
			return (int)cTemp;
	}

	return 0;
}

// RFLAGS reg의 Interrupt Flag(IF)를 변경하고 이전 Interrupt Flag State를 반환
BOOL kSetInterruptFlag(BOOL bEnableInterrupt) {
	QWORD qwRFLAGS;

	// 이전의 RFLAGS reg. 값을 읽은 뒤에 Interrupt 가능/불가 처리
	qwRFLAGS = kReadRFLAGS();
	if(bEnableInterrupt == TRUE)
		kEnableInterrupt();
	else
		kDisableInterrupt();

	// 이전 RFLAGS reg.의 IF bit(bit 9)를 확인하여 이전의 Interrupt State를 반환
	if(qwRFLAGS & 0x0200)
		return TRUE;
	return FALSE;
}
