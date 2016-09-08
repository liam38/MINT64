#include "InterruptHandler.h"
#include "PIC.h"
#include "Keyboard.h"
#include "Console.h"
#include "Utility.h"
#include "Task.h"
#include "Descriptor.h"

// 공용 Exception Handler
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
	kPrintStringXY(70, 0, vcBuffer);
	//============================================================

	// Send EOI.
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);
}

// 타이머 인터럽트 핸들러
void kTimerHandler(int iVectorNumber) {
	char vcBuffer[] = "[INT:  , ]";
	static int g_iTimerInterruptCount = 0;

	//============================================================
	// 인터럽트가 발생했음을 알리려고 메시지를 출력하는 부분
	// 인터럽트 벡터를 화면 오른쪽 위에 2자리 정수로 출력
	vcBuffer[5] = '0' + iVectorNumber / 10;
	vcBuffer[6] = '0' + iVectorNumber % 10;
	// 발생한 횟수 출력
	vcBuffer[8] = '0' + g_iTimerInterruptCount;
	g_iTimerInterruptCount = (g_iTimerInterruptCount + 1) % 10;
	kPrintStringXY(70, 0, vcBuffer);
	//============================================================

	// EOI 전송
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);

	// 타이버 발생 횟수를 증가
	g_qwTickCount++;

	// 태스크가 사용한 프로세서의 시간을 줄임
	kDecreaseProcessorTime();
	// 프로세서가 사용할 수 있는 시간을 다 썼다면 태스크 전환 수행
	if(kIsProcessorTimeExpired() == TRUE)
		kScheduleInInterrupt();
}

// 키보드 인터럽트 핸들러
void kKeyboardHandler(int iVectorNumber) {
	char vcBuffer[] = "[INT:  , ]";
	static int g_iKeyboardInterruptCount = 0;
	BYTE bTemp;

	//============================================================
	// Interrupt 발생 메시지 출력
	// Interrupt Vector 출력
	vcBuffer[5] = '0' + iVectorNumber / 10;
	vcBuffer[6] = '0' + iVectorNumber % 10;
	// Interrupt 발생 횟수 출력
	vcBuffer[8] = '0' + g_iKeyboardInterruptCount;
	g_iKeyboardInterruptCount = (g_iKeyboardInterruptCount + 1) % 10;
	kPrintStringXY(0, 0, vcBuffer);
	//============================================================

	// 키보드 컨트롤러에서 데이터를 읽어서 ASCII로 변환하여 큐에 삽입
	if(kIsOutputBufferFull() == TRUE) {
		bTemp = kGetKeyboardScanCode();
		kConvertScanCodeAndPutQueue(bTemp);
	}

	// Send EOI.
	kSendEOIToPIC(iVectorNumber - PIC_IRQSTARTVECTOR);
}
