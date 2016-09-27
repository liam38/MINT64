#ifndef __TYPES_H__
#define __TYPES_H__

#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long
#define BOOL char

#define TRUE 1
#define FALSE 0
#define NULL 0

// stddef.h 헤더에 포함된 offsetof() 매크로의 내용
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

#pragma pack(push, 1)

//비디오 모드 중 텍스트 모드 화면을 구성하는 자료구조
typedef struct kCharactorStruct{
	BYTE bCharacter;
	BYTE bAttribute;
}CHARACTER;

#pragma pack(pop)
#endif /*__TYPES_H__*/
