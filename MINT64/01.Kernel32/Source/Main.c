#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"

void kPrintString(int iX, int iY, const char* pcString);
BOOL kInitializeKernel64Area(void);
BOOL kIsMemoryEnough(void);
void kCopyKernel64ImageTo2Mbyte(void);

void Main(void) {
	DWORD i;
	DWORD dwEAX, dwEBX, dwECX, dwEDX;
	char vcVendorString[13] = {0};

	kPrintString(0, 3, "Protected Mode C Language Kernel Started....[Pass]");

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

	kReadCPUID(0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	*(DWORD*)vcVendorString = dwEBX;
	*((DWORD*)vcVendorString + 1) = dwEDX;
	*((DWORD*)vcVendorString + 2) = dwECX;
	kPrintString(0, 7, "Processor Vendor String.....................[            ]");
	kPrintString(45, 7, vcVendorString);

	//64bit ���� ���� Ȯ��
	kReadCPUID(0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	kPrintString(0, 8, "64bit Mode Support Check....................[    ]");
	if(dwEDX & (1 << 29))
		kPrintString(45, 8, "Pass");
	else {
		kPrintString(45, 8, "Fail");
		kPrintString(0, 9, "This processor does not support 64bit mode...");
	}

	//IA-32e ��� Ŀ���� 0x200000(2MB) ��巹���� �̵�
	kPrintString(0, 9, "Copy IA-32e Kernel to 2M Address............[    ]");
	kCopyKernel64ImageTo2Mbyte();
	kPrintString(45, 9, "Pass");

	kPrintString(0, 9, "Switch To IA-32e Mode");
	kSwitchAndExecute64bitKernel();
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

		if(*pdwCurrentAddress != 0)
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

void kCopyKernel64ImageTo2Mbyte(void) {
	WORD wKernel32SectorCount, wTotalKernelSectorCount;
	DWORD* pdwSourceAddress, * pdwDestinationAddress;
	int index;

	//0x7C05�� �� Ŀ�� ���� ��, 0x7C07�� ��ȣ��� Ŀ�� ���� ���� ����ִ�.
	wTotalKernelSectorCount = *((WORD*)0x7C05);
	wKernel32SectorCount = *((WORD*)0x7C07);

	pdwSourceAddress =(DWORD*)(0x10000 + (wKernel32SectorCount * 512));
	pdwDestinationAddress = (DWORD*)0x200000;

	//IA-32e ��� Ŀ�� ���� ũ�⸸ŭ ����
	for(index=0; index < 512 * (wTotalKernelSectorCount - wKernel32SectorCount) / 4; index++) {
		*pdwDestinationAddress = *pdwSourceAddress;
		pdwDestinationAddress++;
		pdwSourceAddress++;
	}
}
