#ifndef __TASK_H__
#define __TASK_H__

#include "Types.h"
#include "List.h"

// Macro
// SS, RSP, RFLAGS, CS, RIP + ISR(19�� register)
#define TASK_REGISTERCOUNT 		(5+19)
#define TASK_REGISTERSIZE 		8

// Context �ڷᱸ���� �������� offset
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

// Task Pool �� address
#define TASK_TCBPOOLADDRESS 	0x800000
#define TASK_MAXCOUNT 			1024

// Stack Pool�� Stack Size
#define TASK_STACKPOOLADDRESS 	(TASK_TCBPOOLADDRESS + sizeof(TCB) * TASK_MAXCOUNT)
#define TASK_STACKSIZE 			8192

// ��ȿ���� ���� �½�ũ ID
#define TASK_INVALIDID 			0xFFFFFFFFFFFFFFFF

// �½�ũ�� �ִ�� �� �� �ִ� ���μ��� �ð�(5ms)
#define TASK_PROCESSORTIME 		5

// �غ� ����Ʈ�� ��
#define TASK_MAXREADYLISTCOUNT 	5

// �½�ũ�� �켱����
#define TASK_FLAGS_HIGHEST 		0
#define TASK_FLAGS_HIGH			1
#define TASK_FLAGS_MEDUIM 		2
#define TASK_FLAGS_LOW 			3
#define TASK_FLAGS_LOWEST 		4
#define TASK_FLAGS_WAIT			0xFF

// �½�ũ�� �÷���
#define TASK_FLAGS_ENDTASK 		0x8000000000000000
#define TASK_FLAGS_SYSTEM 		0x4000000000000000
#define TASK_FLAGS_PROCESS 		0x2000000000000000
#define TASK_FLAGS_THREAD 		0x1000000000000000
#define TASK_FLAGS_IDLE 		0x0800000000000000

// ��ũ�� �Լ�
#define GETPRIORITY(x) 				((x) & 0xFF)
#define SETPRIORITY(x, priority) 	((x) = ((x) & 0xFFFFFFFFFFFFFF00) | (priority))
#define GETTCBOFFSET(x) 			((x) & 0xFFFFFFFF)
  // �ڽ� thread link�� ����� stThreadLink �������� �½�ũ �ڷᱸ��(TCB) ��ġ��
  // ����Ͽ� ��ȯ�ϴ� ��ũ��
#define GETTCBFROMTHREADLINK(x) 	(TCB*)((QWORD)(x) - offsetof(TCB, stThreadLink))

// Struct
#pragma pack(push, 1)

// Context�� ���õ� �ڷᱸ��
typedef struct kContextStruct {
	QWORD vqRegister[TASK_REGISTERCOUNT];
}CONTEXT;

// Task�� ���¸� �����ϴ� �ڷᱸ��
typedef struct kTaskControlBlockStruct {
	// ���� �������� ��ġ�� ID
	LISTLINK stLink;

	// �÷��� (���� 8bit�� �켱������ ��Ÿ���� field�� ���)
	QWORD qwFlags;

	// ���μ��� �޸� ������ ���۰� ũ��
	void* pvMemoryAddress;
	QWORD qwMemorySize;

	//=================================================================
	// ���� ������ ����
	//=================================================================
	// �ڽ� �������� ��ġ�� ID
	LISTLINK stThreadLink;
	// �ڽ� �������� ����Ʈ
	LIST stChildThreadList;
	// �θ� ���μ����� ID
	QWORD qwParrentProcessID;

	//Context
	CONTEXT stContext;

	//Stack�� address�� size
	void* pvStackAddress;
	QWORD qwStackSize;
}TCB;

// TCB Pool�� ���¸� �����ϴ� �ڷᱸ��
typedef struct kTCBPoolManagerStruct {
	// Task Pool�� ���� ����
	TCB* pstStartAddress;
	int iMaxCount;
	int iUseCount;

	// TCB�� �Ҵ�� Ƚ��
	int iAllocatedCount;
}TCBPOOLMANAGER;

// �����ٷ��� ���¸� �����ϴ� �ڷᱸ��
typedef struct kSchedulerStruct {
	// ���� �������� Task
	TCB* pstRunningTask;

	// ���� �������� �½�ũ�� ����� �� �ִ� ���μ��� �ð�
	int iProcessorTime;

	// ������ �½�ũ�� �غ� ���� ����Ʈ, �½�ũ�� �켱������ ���� ����
	LIST vstReadyList[TASK_MAXREADYLISTCOUNT];

	// ������ �½�ũ�� ��� ���� ����Ʈ
	LIST stWaitList;

	// �� �켱�������� �½�ũ�� ������ Ƚ���� �����ϴ� �ڷᱸ��
	int viExecuteCount[TASK_MAXREADYLISTCOUNT];

	// ���μ��� ���ϸ� ����ϱ� ���� �ڷᱸ��
	QWORD qwProcessorLoad;

	// IDLE Task���� ����� ���μ��� �ð�
	QWORD qwSpendProcessorTimeInIdleTask;
}SCHEDULER;

#pragma pack(pop)

//�Լ�
//==================================================================
// �½�ũ Ǯ�� �½�ũ ����
//==================================================================
static void kInitializeTCBPool(void);
static TCB* kAllocateTCB(void);
static void kFreeTCB(QWORD qwID);
TCB* kCreateTask(QWORD qwFlags, void* pvMemoryAddress, QWORD qwMemorySize, QWORD qwEntryPointAddress);
static void kSetUpTask(TCB* pstTCB, QWORD qwFlags, QWORD qwEntryPointAddress, void* pvStackAddress, QWORD qwStackSize);

//==================================================================
// �����ٷ� ����
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
// IDLE Task ����
//==================================================================
void kIdleTask(void);
void kHaltProcessorByLoad(void);

#endif /*__TASK_H__*/
