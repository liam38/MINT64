#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"
#include "AssemblyUtility.h"

void kPrintString(int iX, int iY, const char* pcString);

void Main(void) {
	char vcTemp[2] = {0};
	BYTE bTemp;
	int index = 0;
	KEYDATA stData;

	kPrintString(0, 10, "Switch To IA-32e Mode Success");
	kPrintString(0, 11, "IA-32e C Language Kernel Start..............[Pass]");

	kPrintString(0, 12, "GDT Initialize And Switch For IA-32e Mode...[    ]");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_STARTADDRESS);
	kPrintString(45, 12, "Pass");

	kPrintString(0, 13, "TSS Segment Load............................[    ]");
	kLoadTR(GDT_TSSSEGMENT);
	kPrintString(45, 13, "Pass");

	kPrintString(0, 14, "IDT Initialize..............................[    ]");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_STARTADDRESS);
	kPrintString(45, 14, "Pass");

	kPrintString(0, 15, "Keyboard Activate And Queue Initialize......[    ]");

	//키보드 활성화
	if(kInitializeKeyboard() == TRUE) {
		kPrintString(45, 15, "Pass");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else {
		kPrintString(45, 15, "Fail");
		while(1);
	}

	kPrintString(0, 16, "PIC Controller And Interrupt Initialize.....[    ]");
	// PIC controller 초기화 및 모든 Interrupt 활성화
	kInitializePIC();
	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kPrintString(45, 16, "Pass");

	while(1) {
		// 키 큐에 데이터가 있으면 키를 처리
		if(kGetKeyFromKeyQueue(&stData) == TRUE) {
			// 키가 눌러졌으면 키의 ASCII 코드 값을 화면에 출력
			if(stData.bFlags & KEY_FLAGS_DOWN) {
				// 키 데이터의 ASCII 코드 값을 저장
				vcTemp[0] = stData.bASCIICode;
				kPrintString(index++, 17, vcTemp);

				if(vcTemp[0] == '0')
					bTemp = bTemp / 0;
			}
		}
	}
}

void kPrintString(int iX, int iY, const char* pcString) {
	CHARACTER* pstScreen = (CHARACTER*)0xB8000;
	int index;

	pstScreen += (iY * 80) + iX;

	for(index=0; pcString[index] != 0; index++)
		pstScreen[index].bCharacter = pcString[index];
}
