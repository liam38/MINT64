#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"
#include "AssemblyUtility.h"
#include "Console.h"
#include "ConsoleShell.h"

//void kPrintfString(int iX, int iY, const char* pcString);

void Main(void) {
	int iCursorX, iCursorY;

	kInitializeConsole(0, 10);
	kPrintf("Switch To IA-32e Mode Success\n");
	kPrintf("IA-32e C Language Kernel Start..............[Pass]\n");
	kPrintf("Initialize Console..........................[Pass]\n");

	// ���� ��Ȳ ���
	kGetCursor(&iCursorX, &iCursorY);
	kPrintf("GDT Initialize And Switch For IA-32e Mode...[    ]");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_STARTADDRESS);
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	kPrintf("TSS Segment Load............................[    ]");
	kLoadTR(GDT_TSSSEGMENT);
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	kPrintf("IDT Initialize..............................[    ]");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_STARTADDRESS);
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	kPrintf("Total RAM Size Check........................[    ]");
	kCheckTotalRAMSize();
	kSetCursor(45, iCursorY++);
	kPrintf("Pass], Size = %d MB\n", kGetTotalRAMSize());

	kPrintf("Keyboard Activate And Queue Initialize......[    ]");

	//Ű���� Ȱ��ȭ
	if(kInitializeKeyboard() == TRUE) {
		kSetCursor(45, iCursorY++);
		kPrintf("Pass\n");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else {
		kSetCursor(45, iCursorY++);
		kPrintf("Fail\n");
		while(1);
	}

	kPrintf("PIC Controller And Interrupt Initialize.....[    ]");

	// PIC controller �ʱ�ȭ �� ��� Interrupt Ȱ��ȭ
	kInitializePIC();
	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kSetCursor(45, iCursorY++);
	kPrintf("Pass\n");

	// shell�� ����
	kStartConsoleShell();
}

/* kPrintString => kPrintf�� ��ü (Console.h)
void kPrintString(int iX, int iY, const char* pcString) {
	CHARACTER* pstScreen = (CHARACTER*)0xB8000;
	int index;

	pstScreen += (iY * 80) + iX;

	for(index=0; pcString[index] != 0; index++)
		pstScreen[index].bCharacter = pcString[index];
}
*/
