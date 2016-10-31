[BITS 32]

;C에서 호출할 수 있도록 이름을 노출(Export)
global kReadCPUID, kSwitchAndExecute64bitKernel

SECTION .text

;CPUID 반환
;	PARAM: DWORD dwEAX, DWORD* pdwEAX, pdwEBX, pdwECX, pdwEDX
kReadCPUID:
  push ebp
  mov ebp, esp
  push eax
  push ebx
  push ecx
  push edx
  push esi

  ; EAX 값으로 CPUID 명령어 실행
  mov eax, dword [ebp + 8]
  cpuid

  ;반환된 값을 parameter에 저장
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

;IA-32e 모드로 전환, 64bit 커널 수행
;  PARAM 없음
kSwitchAndExecute64bitKernel:
  ;CR4's PAE, OSXMMEXCPT, OSFXSR bit 1로 설정
  mov eax, cr4
  or eax, 0x620 	; PAE (bit 5), OSXMMEXCPT (bit 10), OSFXSR (bit 9) 모두 1로 설정
  mov cr4, eax

  ;CR3 PML4 address, Enable Cache.
  mov eax, 0x100000
  mov cr3, eax

  ;IA32_EFER.LME를 1로 설정 -> IA-32e 모드 활성화
  mov ecx, 0xC0000080
  rdmsr

  or eax, 0x0100

  wrmsr

  ; CR0's NW(bit 29), CD(bit 30)를 0으로 설정 / PG(bit 31)를 1로 설정
  ; 캐시, 페이징 기능 활성화 및 TS(bit 3) = 1, EM(bit 2) = 0, MP(bit 1) = 1로 설정
  ; FPU 활성화
  mov eax, cr0
  or eax, 0xE000000E
  xor eax, 0x60000004
  mov cr0, eax

  jmp 0x08:0x200000

  jmp $
