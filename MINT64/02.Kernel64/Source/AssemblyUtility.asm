[BITS 64]

SECTION .text

; 외부에서 정의된 함수를 쓸 수 있도록 선언(Import)
extern Main

;C언어에서 호출할 수 있도록 이름 노출
global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS
global kReadTSC
global kSwitchContext, kHlt

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 태스크 관련 어셈블리어 함수
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Context를 저장하고 셀렉터를 교체하는 매크로
%macro KSAVECONTEXT 0
	; RBP ~ GS (push)
	push rbp
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov ax, ds		; ds, es는 스택에 직접 push가 불가능. 왜????????
	push rax
	mov ax, es
	push rax
	push fs
	push gs
%endmacro

; Context 복원 매크로
%macro KLOADCONTEXT 0
	; GS ~ RBP (pop)
	pop gs
	pop fs
	pop rax
	mov es, ax
	pop rax
	mov ds, ax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop rbp
%endmacro

; Current Context에 현재 Context를 저장하고 Next Task에서 Context 복구
; 	PARAM : Current Context, Next Context
kSwitchContext:
	push rbp 		; 스택에 RBP 저장, RSP를 RBP에 저장
	mov rbp, rsp

	; Current Context가 Null이면 Context를 저장할 필요 없음
	pushfq 		; 아래의 cmp 결과로 RFLAGS가 변하지 않도록 스택에 저장
	cmp rdi, 0  ; Current Context가 Null 이면 콘텍스트 복원으로 바로 이동
	je LoadContext
	popfq 		; 스택에 저장한 RFLAGS 복원

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; 현재 Task 의 Context를 저장
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push rax 			; Context 영역의 offset으로 사용할 rax를 스택에 저장

	; SS, RSP, RFLAGS, CS, RIP 순으로 삽입
	mov ax, ss 						; SS 저장
	mov qword[rdi + (23 * 8)], rax

	mov rax, rbp 					; RBP에 저장된 RSP 저장
	add rax, 16 					; RSP는 push rbp와 return address를
	mov qword[rdi + (22 * 8)], rax  ; 제외한 값으로 저장

	pushfq							; RFLAGS 저장
	pop rax
	mov qword[rdi + (21 * 8)], rax

	mov ax, cs 						; CS 저장
	mov qword[rdi + (20 * 8)], rax

	mov rax, qword[rbp + 8] 		; RIP를 return address로 설정
	mov qword[rdi + (19 * 8)], rax 	; 다음 콘텍스트 복원 시에 이 함수를 호출한 위치로 이동

	; 저장한 register 복구 뒤 interrupt가 발생했을 때처럼 나머지 context를 모두 저장
	pop rax
	pop rbp

	; 가장 끝 부분에 SS, RSP, RFLAGS, CS, RIP를 저장했으므로, 이전 영역에
	; push 명령으로 콘텍스트를 저장하기 위해 스택을 변경
	add rdi, (19 * 8) 	; rdi에 context 자료구조의 시작 address가 들어있다.
	mov rsp, rdi 		; 이를 스택으로 사용하기 위해 24-5 = 19(번째위치 스택 시작부분)
	sub rdi, (19 * 8)

	; 나버지 레지스터를 모두 Context 자료구조에 저장
	KSAVECONTEXT

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; 다음 태스크의 콘텍스트 복원
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	LoadContext:
		mov rsp, rsi

		; Context 자료구조에서 레지스터를 복원
		KLOADCONTEXT
		iretq

; 프로세서를 쉬게 함
; 	PARAM : 없음
kHlt:
	hlt 	; 프로세서를 대기 상태로 진입시킴
	hlt
	ret
