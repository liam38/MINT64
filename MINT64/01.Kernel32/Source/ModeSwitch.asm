[BITS 32]

;C���� ȣ���� �� �ֵ��� �̸��� ����(Export)
global kReadCPUID, kSwitchAndExecute64bitKernel

SECTION .text

;CPUID ��ȯ
;	PARAM: DWORD dwEAX, DWORD* pdwEAX, pdwEBX, pdwECX, pdwEDX
kReadCPUID:
  push ebp
  mov ebp, esp
  push eax
  push ebx
  push ecx
  push edx
  push esi

  ; EAX ������ CPUID ��ɾ� ����
  mov eax, dword [ebp + 8]
  cpuid

  ;��ȯ�� ���� parameter�� ����
  ;*pdwEAX
  mov esi, dword [ebp + 12]
  mov dword [esi], eax

  ;*pdwEBX
  mov esi, dword [ebp + 16]
  mov dword [esi], ebx

  ;*pdwECX
  mov esi, dword [ebp + 20]
  mov dword [esi], ecx

  ;*pdwEDX
  mov esi, dword [ebp + 24]
  mov dword [esi], edx

  pop esi
  pop edx
  pop ecx
  pop ebx
  pop eax
  pop ebp
  ret

;IA-32e ���� ��ȯ, 64bit Ŀ�� ����
;  PARAM ����
kSwitchAndExecute64bitKernel:
  ;CR4's PAE, OSXMMEXCPT, OSFXSR bit 1�� ����
  mov eax, cr4
  or eax, 0x620 	; PAE (bit 5), OSXMMEXCPT (bit 10), OSFXSR (bit 9) ��� 1�� ����
  mov cr4, eax

  ;CR3 PML4 address, Enable Cache.
  mov eax, 0x100000
  mov cr3, eax

  ;IA32_EFER.LME�� 1�� ���� -> IA-32e ��� Ȱ��ȭ
  mov ecx, 0xC0000080
  rdmsr

  or eax, 0x0100

  wrmsr

  ; CR0's NW(bit 29), CD(bit 30)�� 0���� ���� / PG(bit 31)�� 1�� ����
  ; ĳ��, ����¡ ��� Ȱ��ȭ �� TS(bit 3) = 1, EM(bit 2) = 0, MP(bit 1) = 1�� ����
  ; FPU Ȱ��ȭ
  mov eax, cr0
  or eax, 0xE000000E
  xor eax, 0x60000004
  mov cr0, eax

  jmp 0x08:0x200000

  jmp $
