#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"
#include "AssemblyUtility.h"
#include "Console.h"
#include "ConsoleShell.h"
#include "Task.h"
#include "PIT.h"
#include "DynamicMemory.h"
#include "HardDisk.h"
#include "FileSystem.h"

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

	kPrintf("TCB Pool And Scheduler Initialize...........[Pass]\n");
	iCursorY++;
	kInitializeScheduler();

	// ���� �޸� �ʱ�ȭ
	kPrintf("Dynamic Memory Initialize...................[Pass]\n");
	iCursorY++;
	kInitializeDynamicMemory();

	// 1ms�� �� ���� ���ͷ�Ʈ�� �߻��ϵ��� ����
	kInitializePIT(MSTOCOUNT(1), 1);

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

	// �ϵ� ��ũ�� �ʱ�ȭ
/*	kPrintf("HDD Initialize..............................[    ]");
	if(kInitializeHDD() == TRUE) {
		kSetCursor(45, iCursorY++);
		kPrintf("Pass\n");
	}
	else {
		kSetCursor(45, iCursorY++);
		kPrintf("Fail\n");
	}
*/
	// fs �ʱ�ȭ
	kPrintf("File System Initialize......................[    ]");
	if(kInitializeFileSystem() == TRUE) {
		kSetCursor(45, iCursorY++);
		kPrintf("Pass\n");
	}
	else {
		kSetCursor(45, iCursorY++);
		kPrintf("Fail\n");
	}

	// Idle Task�� �����ϰ� shell�� ����
	kCreateTask(TASK_FLAGS_LOWEST | TASK_FLAGS_THREAD | TASK_FLAGS_SYSTEM | TASK_FLAGS_IDLE, 0, 0, (QWORD)kIdleTask);
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
