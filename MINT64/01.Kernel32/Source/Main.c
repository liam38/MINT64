#include "Types.h"

void kPrintString(int iX, int iY, const char* pcString);

void Main(void) {
	kPrintString(0, 3, "C Language Kernel Started..");
	while(1);
}

void kPrintString(int iX, int iY, const char* pcString) {
	CHARACTER* pstScreen = (CHARACTER*)0xB8000;
	int index;

	pstScreen += (iY * 80) + iX;
	for(index=0; pcString[index] != 0; index++)
		pstScreen[index].bCharacter = pcString[index];
}
