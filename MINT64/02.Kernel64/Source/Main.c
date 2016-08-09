#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"

void kPrintString(int iX, int iY, const char* pcString);

void Main(void) {
	char vcTemp[2] = {0};
	BYTE bFlags;
	BYTE bTemp;
	int index = 0;

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

	kPrintString(0, 15, "Keyboard Activate...........................[    ]");

	//키보드 활성화
	if(kActivateKeyboard() == TRUE) {
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
		//출력 버퍼(0x60)가 차 잇으면 스캔코드를 읽을 수 있음
		if(kIsOutputBufferFull() == TRUE) {
			//출력버퍼(0x60)에서 스캔코드를 읽어서 저장
			bTemp = kGetKeyboardScanCode();

			//스캔코드를 ASCII 코드로 변환하는 함수를 호출하여 ASCII 코드와
			// 눌림 또는 떨어짐 정보를 반환
			if(kConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]), &bFlags) == TRUE) {
				//키가 눌러졌으면 키의 ASCII 코드 값을 화면에 출력
				if(bFlags & KEY_FLAGS_DOWN) {
					kPrintString(index++, 17, vcTemp);

					if(vcTemp[0] == '0')
						bTemp = bTemp / 0;
				}
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
