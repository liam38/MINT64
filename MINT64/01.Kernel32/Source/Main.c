#include "Types.h"
#include "Page.h"

void kPrintString(int iX, int iY, const char* pcString);
BOOL kInitializeKernel64Area(void);
BOOL kIsMemoryEnough(void);

void Main(void) {
	DWORD i;

	kPrintString(0, 3, "C Language Kernel Started...................[Pass]");

	kPrintString(0, 4, "Minimum Memory Size Check...................[    ]");
	if(kIsMemoryEnough() == FALSE) {
		kPrintString(45, 4, "Fail");
		kPrintString(0, 5, "Not Enough Memory. MINT64 OS Requires Over 64MB Memory.");
		while(1);
	}
	else
		kPrintString(45, 4, "Pass");

	//IA-32e ����� Ŀ�� ������ �ʱ�ȭ
	kPrintString(0, 5, "IA-32e Kernel Area Initialize...............[    ]");
	if(kInitializeKernel64Area() == FALSE) {
		kPrintString(45, 5, "Fail");
		kPrintString(0, 6, "Kernel Area Initialization Fail.");
		while(1);
	}
	kPrintString(45, 5, "Pass");

	//IA-32e ��� Ŀ���� ���� ������ ���̺� ����
	kPrintString(0, 6, "IA-32e Page Tables Initialize...............[    ]");
	kInitializePageTables();
	kPrintString(45, 6, "Pass");

	while(1);
}

void kPrintString(int iX, int iY, const char* pcString) {
	CHARACTER* pstScreen = (CHARACTER*)0xB8000;
	int index;

	pstScreen += (iY * 80) + iX;
	for(index=0; pcString[index] != 0; index++)
		pstScreen[index].bCharacter = pcString[index];
}

BOOL kInitializeKernel64Area(void) {
	DWORD* pdwCurrentAddress;

	//�ʱ�ȭ�� ������ ��巹���� 0x100000(1MB)�� ����
	pdwCurrentAddress = (DWORD*)0x100000;

	//������ ��巹���� 0x600000(6MB)���� loop ���鼭 4byte�� 0���� ä��
	while((DWORD)pdwCurrentAddress < 0x600000) {
		*pdwCurrentAddress = 0x00;

		if(*pdwCurrentAddress !=
				0)
			return FALSE;
		pdwCurrentAddress++;
	}

	return TRUE;
}

BOOL kIsMemoryEnough(void) {
	DWORD* pdwCurrentAddress;

	//1MB���� �˻� ����
	pdwCurrentAddress = (DWORD*)0x100000;

	//64MB���� ������ ���鼭 Ȯ��
	while((DWORD)pdwCurrentAddress < 0x4000000) {
		*pdwCurrentAddress = 0x12345678;

		if(*pdwCurrentAddress != 0x12345678)
			return FALSE;

		pdwCurrentAddress += ( 0x100000 / 4 );
	}

	return TRUE;
}
