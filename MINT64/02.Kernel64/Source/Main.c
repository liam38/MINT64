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

	//Ű���� Ȱ��ȭ
	if(kInitializeKeyboard() == TRUE) {
		kPrintString(45, 15, "Pass");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else {
		kPrintString(45, 15, "Fail");
		while(1);
	}

	kPrintString(0, 16, "PIC Controller And Interrupt Initialize.....[    ]");
	// PIC controller �ʱ�ȭ �� ��� Interrupt Ȱ��ȭ
	kInitializePIC();
	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kPrintString(45, 16, "Pass");

	while(1) {
		// Ű ť�� �����Ͱ� ������ Ű�� ó��
		if(kGetKeyFromKeyQueue(&stData) == TRUE) {
			// Ű�� ���������� Ű�� ASCII �ڵ� ���� ȭ�鿡 ���
			if(stData.bFlags & KEY_FLAGS_DOWN) {
				// Ű �������� ASCII �ڵ� ���� ����
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
