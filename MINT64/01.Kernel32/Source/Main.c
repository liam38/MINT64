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

	//IA-32e 모드의 커널 영역을 초기화
	kPrintString(0, 5, "IA-32e Kernel Area Initialize...............[    ]");
	if(kInitializeKernel64Area() == FALSE) {
		kPrintString(45, 5, "Fail");
		kPrintString(0, 6, "Kernel Area Initialization Fail.");
		while(1);
	}
	kPrintString(45, 5, "Pass");

	//IA-32e 모드 커널을 위한 페이지 테이블 생성
	kPrintString(0, 6, "IA-32e Page Tables Initialize...............[    ]");
	kInitializePageTables();
	kPrintString(45, 6, "Pass");

	kReadCPUID(0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	*(DWORD*)vcVendorString = dwEBX;
	*((DWORD*)vcVendorString + 1) = dwEDX;
	*((DWORD*)vcVendorString + 2) = dwECX;
	kPrintString(0, 7, "Processor Vendor String.....................[            ]");
	kPrintString(45, 7, vcVendorString);

	//64bit 지원 유무 확인
	kReadCPUID(0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	kPrintString(0, 8, "64bit Mode Support Check....................[    ]");
	if(dwEDX & (1 << 29))
		kPrintString(45, 8, "Pass");
	else {
		kPrintString(45, 8, "Fail");
		kPrintString(0, 9, "This processor does not support 64bit mode...");
	}

	//IA-32e 모드 커널을 0x200000(2MB) 어드레스로 이동
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

	//초기화를 시작할 어드레스인 0x100000(1MB)을 설정
	pdwCurrentAddress = (DWORD*)0x100000;

	//마지막 어드레스인 0x600000(6MB)까지 loop 돌면서 4byte씩 0으로 채움
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

	//1MB부터 검사 시작
	pdwCurrentAddress = (DWORD*)0x100000;

	//64MB까지 루프를 돌면서 확인
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

	//0x7C05에 총 커널 섹터 수, 0x7C07에 보호모드 커널 섹터 수가 들어있다.
	wTotalKernelSectorCount = *((WORD*)0x7C05);
	wKernel32SectorCount = *((WORD*)0x7C07);

	pdwSourceAddress =(DWORD*)(0x10000 + (wKernel32SectorCount * 512));
	pdwDestinationAddress = (DWORD*)0x200000;

	//IA-32e 모드 커널 섹터 크기만큼 복사
	for(index=0; index < 512 * (wTotalKernelSectorCount - wKernel32SectorCount) / 4; index++) {
		*pdwDestinationAddress = *pdwSourceAddress;
		pdwDestinationAddress++;
		pdwSourceAddress++;
	}
}
