[BITS 64]

SECTION .text

;C언어에서 호출할 수 있도록 이름 노출
global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS
global kReadTSC

; port로부터 1byte 읽음
;  PARAM: port No.
kInPortByte:
	push rdx		; 함수에서 임시로 사용하는 레지스터를 스택에 저장
					; 함수의 마지막 부분에서 스택에 삽입된 값을 꺼내 복원

	mov rdx, rdi	; RDX = 파라미터 1(포트번호)
	mov rax, 0 		; RAX 초기화
	in al, dx 		; dx에 저장된 포트 addr.에서 한 byte를 읽어
					; al에 저장, al은 함수의 반환 값으로 사용

	pop rdx			; 함수에서 사용이 끝난 reg 복원
	ret

; port에 1byte 씀
;  PARAM: port No, data
kOutPortByte:
	push rdx 		; 함수에서 임시로 사용하는 reg를 스택에 저장
	push rax 		; 함수의 마지막 부분에서 스택에 삽입된 값을 꺼내 복원

	mov rdx, rdi 	; RDX = 파라미터 1(포트번호)
	mov rax, rsi 	; RAX = 파라미터 2(데이터)
	out dx, al 		; dx에 저장된 port addr에 al에 저장된 한 바이트 write

	pop rax 		; 함수에서 사용이 끝난 reg 복원
	pop rdx
	ret

; GDTR reg.에 GDT 테이블 설정
; PARAM : GDT 테이블의 정보를 저장하는 자료구조의 addr.
kLoadGDTR:
	lgdt[rdi]		; 파라미터 1(GDTR의 addr)를 프로세서에 로드, gdt 테이블 설정
	ret

; TR reg.에 TSS descriptor 설정
; PARAM : TSS descriptor offset
kLoadTR:
	ltr di			; 파라미터 1(TSS offset)을 프로세서에 설정, tss segment 로드
	ret

; IDTR reg.에 IDT 테이블 설정
; PARAM : IDT table의 정보를 저장하는 자료구조의 addr
kLoadIDTR:
	lidt[rdi]
	ret

; Enable Interrupt
;	PARAM : 없음
kEnableInterrupt:
	sti
	ret

; Disable Interrupt
;	PARAN : 없음
kDisableInterrupt:
	cli
	ret

; RFLAGS reg load
;	PARAM : 없음
kReadRFLAGS:
	pushfq
	pop rax

	ret

; 타임 스탬프 카운터를 읽어서 반환
; 	PARAM : 없음
kReadTSC:
	push rdx

	rdtsc				; 타임 스탬프 카운터를 읽어서 RDX:RAX에 저장

	shl rdx, 32			; RDX 에 있는 상위 32bit TSC 값과 RAX에 있는
	or rax, rdx			; 하위 32bit TSC 값을 OR 연산. RAX에 64bit TSC값 저장

	pop rdx
	ret
