#include "Utility.h"

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
