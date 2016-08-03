#include "Utility.h"

//�޸𸮸� Ư�� ������ ä��
void kMemSet(void* pvDestination, BYTE bData, int iSize) {
	int index;

	for(index = 0; index < iSize; index++)
		((char*)pvDestination)[index] = bData;
}

//�޸� ����
int kMemCpy(void* pvDestination, const void* pvSource, int iSize) {
	int index;

	for(index = 0; index < iSize; index++)
		((char*)pvDestination)[index] = ((char*)pvSource)[index];

	return iSize;
}

//�޸� ��
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
