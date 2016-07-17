#include "Page.h"

void kInitializePageTables(void) {
	PML4TENTRY* pstPML4TEntry;
	PDPTENTRY* pstPDPTEntry;
	PDENTRY* pstPDEntry;
	DWORD dwMappingAddress;
	int index;

	//PML4 ���̺� ����
	//ù ��° Entry�� �����ϰ� ��� 0���� �ʱ�ȭ
	pstPML4TEntry = (PML4TENTRY*)0x100000;
	kSetPageEntryData(&(pstPML4TEntry[0]), 0x00, 0x101000, PAGE_FLAGS_DEFAULT, 0);
	for(index=1; index < PAGE_MAXENTRYCOUNT; index++)
		kSetPageEntryData(&(pstPML4TEntry[index]), 0, 0, 0, 0);

	//������ ���͸� ����Ʈ ���̺� ����
	//�ϳ��� PDPT�� 512GB���� ���� ����
	//64�� ��Ʈ�� ���� => 64GB���� ����
	pstPDPTEntry = (PDPTENTRY*)0x101000;
	for(index=0; index < 64; index++)
		kSetPageEntryData(&(pstPDPTEntry[index]), 0, 0x102000 + (index * PAGE_TABLESIZE),
				PAGE_FLAGS_DEFAULT, 0);

	for(index = 64; index< PAGE_MAXENTRYCOUNT; index++)
		kSetPageEntryData(&(pstPDPTEntry[index]), 0, 0, 0, 0);

	//������ ���͸� ���̺� ����
	//�ϳ��� ������ ���͸��� 1GB���� ���� ����
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
