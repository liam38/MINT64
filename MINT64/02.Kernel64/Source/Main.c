#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"

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

	//Ű���� Ȱ��ȭ
	if(kActivateKeyboard() == TRUE) {
		kPrintString(45, 15, "Pass");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else {
		kPrintString(45, 15, "Fail");
		while(1);
	}

	while(1) {
		//��� ����(0x60)�� �� ������ ��ĵ�ڵ带 ���� �� ����
		if(kIsOutputBufferFull() == TRUE) {
			//��¹���(0x60)���� ��ĵ�ڵ带 �о ����
			bTemp = kGetKeyboardScanCode();

			//��ĵ�ڵ带 ASCII �ڵ�� ��ȯ�ϴ� �Լ��� ȣ���Ͽ� ASCII �ڵ��
			// ���� �Ǵ� ������ ������ ��ȯ
			if(kConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]), &bFlags) == TRUE) {
				//Ű�� ���������� Ű�� ASCII �ڵ� ���� ȭ�鿡 ���
				if(bFlags & KEY_FLAGS_DOWN) {
					kPrintString(index++, 16, vcTemp);

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