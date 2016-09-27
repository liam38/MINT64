#ifndef __TASK_H__
#define __TASK_H__

#include "Types.h"
#include "List.h"

// Macro
// SS, RSP, RFLAGS, CS, RIP + ISR(19개 register)
#define TASK_REGISTERCOUNT 		(5+19)
#define TASK_REGISTERSIZE 		8

// Context 자료구조의 레지스터 offset
#define TASK_GSOFFSET 			0
#define TASK_FSOFFSET 			1
#define TASK_ESOFFSET 			2
#define TASK_DSOFFSET 			3
#define TASK_R15OFFSET 			4
#define TASK_R14OFFSET 			5
#define TASK_R13OFFSET 			6
#define TASK_R12OFFSET 			7
#define TASK_R11OFFSET 			8
#define TASK_R10OFFSET 			9
#define TASK_R9OFFSET 			10
#define TASK_R8OFFSET 			11
#define TASK_RSIOFFSET 			12
#define TASK_RDIOFFSET 			13
#define TASK_RDXOFFSET 			14
#define TASK_RCXOFFSET 			15
#define TASK_RBXOFFSET 			16
#define TASK_RAPOFFSET 			17
#define TASK_RBPOFFSET 			18
#define TASK_RIPOFFSET 			19
#define TASK_CSOFFSET 			20
#define TASK_RFLAGSOFFSET 		21
#define TASK_RSPOFFSET 			22
#define TASK_SSOFFSET 			23

// Task Pool 의 address
#define TASK_TCBPOOLADDRESS 	0x800000
#define TASK_MAXCOUNT 			1024

// Stack Pool과 Stack Size
#define TASK_STACKPOOLADDRESS 	(TASK_TCBPOOLADDRESS + sizeof(TCB) * TASK_MAXCOUNT)
#define TASK_STACKSIZE 			8192

// 유효하지 않은 태스크 ID
#define TASK_INVALIDID 			0xFFFFFFFFFFFFFFFF

// 태스크가 최대로 쓸 수 있는 프로세서 시간(5ms)
#define TASK_PROCESSORTIME 		5

// 준비 리스트의 수
#define TASK_MAXREADYLISTCOUNT 	5

// 태스크의 우선순위
#define TASK_FLAGS_HIGHEST 		0
#define TASK_FLAGS_HIGH			1
#define TASK_FLAGS_MEDUIM 		2
#define TASK_FLAGS_LOW 			3
#define TASK_FLAGS_LOWEST 		4
#define TASK_FLAGS_WAIT			0xFF

// 태스크의 플래그
#define TASK_FLAGS_ENDTASK 		0x8000000000000000
#define TASK_FLAGS_SYSTEM 		0x4000000000000000
#define TASK_FLAGS_PROCESS 		0x2000000000000000
#define TASK_FLAGS_THREAD 		0x1000000000000000
#define TASK_FLAGS_IDLE 		0x0800000000000000

// 매크로 함수
#define GETPRIORITY(x) 				((x) & 0xFF)
#define SETPRIORITY(x, priority) 	((x) = ((x) & 0xFFFFFFFFFFFFFF00) | (priority))
#define GETTCBOFFSET(x) 			((x) & 0xFFFFFFFF)
  // 자식 thread link에 연결된 stThreadLink 정보에서 태스크 자료구조(TCB) 위치를
  // 계산하여 반환하는 매크로
#define GETTCBFROMTHREADLINK(x) 	(TCB*)((QWORD)(x) - offsetof(TCB, stThreadLink))

// Struct
#pragma pack(push, 1)

// Context에 관련된 자료구조
typedef struct kContextStruct {
	QWORD vqRegister[TASK_REGISTERCOUNT];
}CONTEXT;

// Task의 상태를 관리하는 자료구조
typedef struct kTaskControlBlockStruct {
	// 다음 데이터의 위치와 ID
	LISTLINK stLink;

	// 플래그 (하위 8bit는 우선순위를 나타내는 field로 사용)
	QWORD qwFlags;

	// 프로세스 메모리 영역의 시작과 크기
	void* pvMemoryAddress;
	QWORD qwMemorySize;

	//=================================================================
	// 이하 스레드 정보
	//=================================================================
	// 자식 스레드의 위치와 ID
	LISTLINK stThreadLink;
	// 자식 스레드의 리스트
	LIST stChildThreadList;
	// 부모 프로세스의 ID
	QWORD qwParrentProcessID;

	//Context
	CONTEXT stContext;

	//Stack의 address와 size
	void* pvStackAddress;
	QWORD qwStackSize;
}TCB;

// TCB Pool의 상태를 관리하는 자료구조
typedef struct kTCBPoolManagerStruct {
	// Task Pool에 대한 정보
	TCB* pstStartAddress;
	int iMaxCount;
	int iUseCount;

	// TCB가 할당된 횟수
	int iAllocatedCount;
}TCBPOOLMANAGER;

// 스케줄러의 상태를 관리하는 자료구조
typedef struct kSchedulerStruct {
	// 현재 수행중인 Task
	TCB* pstRunningTask;

	// 현재 수행중인 태스크가 사용할 수 있는 프로세서 시간
	int iProcessorTime;

	// 실행할 태스크가 준비 중인 리스트, 태스크의 우선순위에 따라 구분
	LIST vstReadyList[TASK_MAXREADYLISTCOUNT];

	// 종료할 태스크가 대기 중인 리스트
	LIST stWaitList;

	// 각 우선순위별로 태스크를 실행한 횟수를 저장하는 자료구조
	int viExecuteCount[TASK_MAXREADYLISTCOUNT];

	// 프로세서 부하를 계산하기 위한 자료구조
	QWORD qwProcessorLoad;

	// IDLE Task에서 사용한 프로세서 시간
	QWORD qwSpendProcessorTimeInIdleTask;
}SCHEDULER;

#pragma pack(pop)

//함수
//==================================================================
// 태스크 풀과 태스크 관련
//==================================================================
static void kInitializeTCBPool(void);
static TCB* kAllocateTCB(void);
static void kFreeTCB(QWORD qwID);
TCB* kCreateTask(QWORD qwFlags, void* pvMemoryAddress, QWORD qwMemorySize, QWORD qwEntryPointAddress);
static void kSetUpTask(TCB* pstTCB, QWORD qwFlags, QWORD qwEntryPointAddress, void* pvStackAddress, QWORD qwStackSize);

//==================================================================
// 스케줄러 관련
//==================================================================
void kInitializeScheduler(void);
void kSetRunningTask(TCB* pstTask);
TCB* kGetRunningTask(void);
static TCB* kGetNextTaskToRun(void);
static BOOL kAddTaskToReadyList(TCB* pstTask);
void kSchedule(void);
BOOL kScheduleInInterrupt(void);
void kDecreaseProcessorTime(void);
BOOL kIsProcessorTimeExpired(void);
static TCB* kRemoveTaskFromReadyList(QWORD qwTaskID);
BOOL kChangePriority(QWORD qwID, BYTE bPriority);
BOOL kEndTask(QWORD qwTaskID);
void kExitTask(void);
int kGetReadyTaskCount(void);
int kGetTaskCount(void);
TCB* kGetTCBInTCBPool(int iOffset);
BOOL kIsTaskExist(QWORD qwID);
QWORD kGetProcessorLoad(void);
static TCB* kGetProcessByThread(TCB* pstThread);

//==================================================================
// IDLE Task 관련
//==================================================================
void kIdleTask(void);
void kHaltProcessorByLoad(void);

#endif /*__TASK_H__*/
