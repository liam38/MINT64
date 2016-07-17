#include "Page.h"

void kInitializePageTables(void) {
	PML4TENTRY* pstPML4TEntry;
	PDPTENTRY* pstPDPTEntry;
	PDENTRY* pstPDEntry;
	DWORD dwMappingAddress;
	int index;

	//PML4 테이블 생성
	//첫 번째 Entry를 제외하고 모두 0으로 초기화
	pstPML4TEntry = (PML4TENTRY*)0x100000;
	kSetPageEntryData(&(pstPML4TEntry[0]), 0x00, 0x101000, PAGE_FLAGS_DEFAULT, 0);
	for(index=1; index < PAGE_MAXENTRYCOUNT; index++)
		kSetPageEntryData(&(pstPML4TEntry[index]), 0, 0, 0, 0);

	//페이지 디렉터리 포인트 테이블 생성
	//하나의 PDPT로 512GB까지 매핑 가능
	//64개 엔트리 설정 => 64GB까지 매핑
	pstPDPTEntry = (PDPTENTRY*)0x101000;
	for(index=0; index < 64; index++)
		kSetPageEntryData(&(pstPDPTEntry[index]), 0, 0x102000 + (index * PAGE_TABLESIZE),
				PAGE_FLAGS_DEFAULT, 0);

	for(index = 64; index< PAGE_MAXENTRYCOUNT; index++)
		kSetPageEntryData(&(pstPDPTEntry[index]), 0, 0, 0, 0);

	//페이지 디렉터리 테이블 생성
	//하나의 페이지 디렉터리로 1GB까지 매핑 가능
	pstPDEntry = (PDENTRY*)0x102000;
	dwMappingAddress = 0;
	for(index=0; index < PAGE_MAXENTRYCOUNT * 64; index++) {
		kSetPageEntryData(&(pstPDEntry[index]), (index * (PAGE_DEFAULTSIZE >> 20)) >> 12,
				dwMappingAddress, PAGE_FLAGS_DEFAULT | PAGE_FLAGS_PS, 0);
		dwMappingAddress += PAGE_DEFAULTSIZE;
	}
}

void kSetPageEntryData(PTENTRY* pstEntry, DWORD dwUpperBaseAddress,
		DWORD dwLowerBaseAddress, DWORD dwLowerFlags, DWORD dwUpperFlags) {
	pstEntry->dwAttributeAndLowerBaseAddress = dwLowerBaseAddress | dwLowerFlags;
	pstEntry->dwUpperBaseAddressAndEXB = (dwUpperBaseAddress & 0xFF) | dwUpperFlags;
}
