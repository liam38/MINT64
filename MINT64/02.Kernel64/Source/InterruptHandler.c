#include "InterruptHandler.h"
#include "PIC.h"

// 공용 Exception Handler
void kCommonExceptionHandler(int iVectorNumber, QWORD qwErrorCode) {
	char vcBuffer[3] = {0};

	// Printing Interrupt Vector
	vcBuffer[0] = '0' + iVectorNumber / 10;
	vcBuffer[1] = '0' + iVectorNumber % 10;

	kPrintString(0, 0, "======================================================");
	kPrintString(0, 1, "                 Exception Occur                      ");
	kPrintString(0, 2, "                   Vector:                           ");
	kPrintString(27, 2, vcBuffer);
	kPrintString(0, 3, "======================================================");

	//while(1);
}

// 공용 Interrupt Handler
void kCommonInterruptHandler(int iVectorNumber) {
	char vcBuffer[] = "[INT:  , ]";
	static int g_iCommonInterruptCount = 0;

	//============================================================
	// Interrupt 발생 메시지 출력
	// Interrupt Vector 출력
	vcBuffer[5] = '0' + iVectorNumber / 10;
	vcBuffer[6] = '0' + iVectorNumber % 10;
	// Interrupt 발생 횟수 출력
	vcBuffer[8] = '0' + g_iCommonInterruptCount;
	g_iCommonInterruptCount = (g_iCommonInterruptCount + 1) % 10;
	kPrintString(70, 0, vcBuffer);
	//============================================================

	// Send EOI.
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);
}

// 키보드 인터럽트 핸들러
void kKeyboardHandler(int iVectorNumber) {
	char vcBuffer[] = "[INT:  , ]";
	static int g_iKeyboardInterruptCount = 0;

	//============================================================
	// Interrupt 발생 메시지 출력
	// Interrupt Vector 출력
	vcBuffer[5] = '0' + iVectorNumber / 10;
	vcBuffer[6] = '0' + iVectorNumber % 10;
	// Interrupt 발생 횟수 출력
	vcBuffer[8] = '0' + g_iKeyboardInterruptCount;
	g_iKeyboardInterruptCount = (g_iKeyboardInterruptCount + 1) % 10;
	kPrintString(0, 0, vcBuffer);
	//============================================================

	// Send EOI.
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);
}
