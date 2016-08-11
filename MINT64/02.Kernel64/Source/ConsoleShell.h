#ifndef __CONSOLESHELL_H__
#define __CONSOLESHELL_H__

#include "Types.h"

// macro
#define CONSOLESHELL_MAXCOMMANDBUFFERCOUNT 	300
#define CONSOLESHELL_PROMPTMESSAGE 			"PCJ>"

// ���ڿ� �����͸� �Ķ���ͷ� �޴� �Լ� ������ Ÿ�� ����
typedef void(*CommandFunction) (const char* pcParameter);

#pragma pack(push,1)

// Shell�� Ŀ�ǵ带 �����ϴ� �ڷᱸ��
typedef struct kShellCommandEntryStruct {
	// Ŀ�ǵ� ���ڿ�
	char* pcCommand;
	// Ŀ�ǵ� ����
	char* pcHelp;
	// Ŀ�ǵ带 �����ϴ� �Լ��� ������
	CommandFunction pfFunction;
}SHELLCOMMANDENTRY;

// �Ķ���͸� ó���ϱ� ���� ������ �����ϴ� �ڷᱸ��
typedef struct kParameterListStruct {
	// �Ķ���� ������ address
	const char* pcBuffer;
	// �Ķ������ ����
	int iLength;
	// ���� ó���� �Ķ���Ͱ� �����ϴ� ��ġ
	int iCurrentPosition;
}PARAMETERLIST;

#pragma pack(pop)

// function
// ���� shell code
void kStartConsoleShell(void);
void kExecuteCommand(const char* pcCommandBuffer);
void kInitializeParameter(PARAMETERLIST* pstList, const char* pcParameter);
int kGetNextParameter(PARAMETERLIST* pstList, char* pcParameter);

// Ŀ�ǵ带 ó���ϴ� �Լ�
void kHelp(const char* pcParameterBuffer);
void kCls(const char* pcParameterBuffer);
void kShowTotalRAMSize(const char* pcParameterBuffer);
void kStringToDecimalHexTest(const char* pcParameterBuffer);
void kShutdown(const char* pcParameterBuffer);

#endif /*__CONSOLESHELL_H__*/