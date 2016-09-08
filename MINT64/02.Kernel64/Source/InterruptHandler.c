#include "InterruptHandler.h"
#include "PIC.h"
#include "Keyboard.h"
#include "Console.h"
#include "Utility.h"
#include "Task.h"
#include "Descriptor.h"

// ���� Exception Handler
void kCommonExceptionHandler(int iVectorNumber, QWORD qwErrorCode) {
	char vcBuffer[3] = {0};

	// Printing Interrupt Vector
	vcBuffer[0] = '0' + iVectorNumber / 10;
	vcBuffer[1] = '0' + iVectorNumber % 10;

	kPrintStringXY(0, 0, "======================================================");
	kPrintStringXY(0, 1, "                 Exception Occur                      ");
	kPrintStringXY(0, 2, "                   Vector:                           ");
	kPrintStringXY(27, 2, vcBuffer);
	kPrintStringXY(0, 3, "======================================================");

	//while(1);
}

// ���� Interrupt Handler
void kCommonInterruptHandler(int iVectorNumber) {
	char vcBuffer[] = "[INT:  , ]";
	static int g_iCommonInterruptCount = 0;

	//============================================================
	// Interrupt �߻� �޽��� ���
	// Interrupt Vector ���
	vcBuffer[5] = '0' + iVectorNumber / 10;
	vcBuffer[6] = '0' + iVectorNumber % 10;
	// Interrupt �߻� Ƚ�� ���
	vcBuffer[8] = '0' + g_iCommonInterruptCount;
	g_iCommonInterruptCount = (g_iCommonInterruptCount + 1) % 10;
	kPrintStringXY(70, 0, vcBuffer);
	//============================================================

	// Send EOI.
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);
}

// Ÿ�̸� ���ͷ�Ʈ �ڵ鷯
void kTimerHandler(int iVectorNumber) {
	char vcBuffer[] = "[INT:  , ]";
	static int g_iTimerInterruptCount = 0;

	//============================================================
	// ���ͷ�Ʈ�� �߻������� �˸����� �޽����� ����ϴ� �κ�
	// ���ͷ�Ʈ ���͸� ȭ�� ������ ���� 2�ڸ� ������ ���
	vcBuffer[5] = '0' + iVectorNumber / 10;
	vcBuffer[6] = '0' + iVectorNumber % 10;
	// �߻��� Ƚ�� ���
	vcBuffer[8] = '0' + g_iTimerInterruptCount;
	g_iTimerInterruptCount = (g_iTimerInterruptCount + 1) % 10;
	kPrintStringXY(70, 0, vcBuffer);
	//============================================================

	// EOI ����
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);

	// Ÿ�̹� �߻� Ƚ���� ����
	g_qwTickCount++;

	// �½�ũ�� ����� ���μ����� �ð��� ����
	kDecreaseProcessorTime();
	// ���μ����� ����� �� �ִ� �ð��� �� ��ٸ� �½�ũ ��ȯ ����
	if(kIsProcessorTimeExpired() == TRUE)
		kScheduleInInterrupt();
}

// Ű���� ���ͷ�Ʈ �ڵ鷯
void kKeyboardHandler(int iVectorNumber) {
	char vcBuffer[] = "[INT:  , ]";
	static int g_iKeyboardInterruptCount = 0;
	BYTE bTemp;

	//============================================================
	// Interrupt �߻� �޽��� ���
	// Interrupt Vector ���
	vcBuffer[5] = '0' + iVectorNumber / 10;
	vcBuffer[6] = '0' + iVectorNumber % 10;
	// Interrupt �߻� Ƚ�� ���
	vcBuffer[8] = '0' + g_iKeyboardInterruptCount;
	g_iKeyboardInterruptCount = (g_iKeyboardInterruptCount + 1) % 10;
	kPrintStringXY(0, 0, vcBuffer);
	//============================================================

	// Ű���� ��Ʈ�ѷ����� �����͸� �о ASCII�� ��ȯ�Ͽ� ť�� ����
	if(kIsOutputBufferFull() == TRUE) {
		bTemp = kGetKeyboardScanCode();
		kConvertScanCodeAndPutQueue(bTemp);
	}

	// Send EOI.
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);
}
