[BITS 64]

SECTION .text

; �ܺο��� ���ǵ� �Լ��� �� �� �ֵ��� ����(Import)
extern Main

;C���� ȣ���� �� �ֵ��� �̸� ����
global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS
global kReadTSC
global kSwitchContext, kHlt

; port�κ��� 1byte ����
;  PARAM: port No.
kInPortByte:
	push rdx		; �Լ����� �ӽ÷� ����ϴ� �������͸� ���ÿ� ����
					; �Լ��� ������ �κп��� ���ÿ� ���Ե� ���� ���� ����

	mov rdx, rdi	; RDX = �Ķ���� 1(��Ʈ��ȣ)
	mov rax, 0 		; RAX �ʱ�ȭ
	in al, dx 		; dx�� ����� ��Ʈ addr.���� �� byte�� �о�
					; al�� ����, al�� �Լ��� ��ȯ ������ ���

	pop rdx			; �Լ����� ����� ���� reg ����
	ret

; port�� 1byte ��
;  PARAM: port No, data
kOutPortByte:
	push rdx 		; �Լ����� �ӽ÷� ����ϴ� reg�� ���ÿ� ����
	push rax 		; �Լ��� ������ �κп��� ���ÿ� ���Ե� ���� ���� ����

	mov rdx, rdi 	; RDX = �Ķ���� 1(��Ʈ��ȣ)
	mov rax, rsi 	; RAX = �Ķ���� 2(������)
	out dx, al 		; dx�� ����� port addr�� al�� ����� �� ����Ʈ write

	pop rax 		; �Լ����� ����� ���� reg ����
	pop rdx
	ret

; GDTR reg.�� GDT ���̺� ����
; PARAM : GDT ���̺��� ������ �����ϴ� �ڷᱸ���� addr.
kLoadGDTR:
	lgdt[rdi]		; �Ķ���� 1(GDTR�� addr)�� ���μ����� �ε�, gdt ���̺� ����
	ret

; TR reg.�� TSS descriptor ����
; PARAM : TSS descriptor offset
kLoadTR:
	ltr di			; �Ķ���� 1(TSS offset)�� ���μ����� ����, tss segment �ε�
	ret

; IDTR reg.�� IDT ���̺� ����
; PARAM : IDT table�� ������ �����ϴ� �ڷᱸ���� addr
kLoadIDTR:
	lidt[rdi]
	ret

; Enable Interrupt
;	PARAM : ����
kEnableInterrupt:
	sti
	ret

; Disable Interrupt
;	PARAN : ����
kDisableInterrupt:
	cli
	ret

; RFLAGS reg load
;	PARAM : ����
kReadRFLAGS:
	pushfq
	pop rax

	ret

; Ÿ�� ������ ī���͸� �о ��ȯ
; 	PARAM : ����
kReadTSC:
	push rdx

	rdtsc				; Ÿ�� ������ ī���͸� �о RDX:RAX�� ����

	shl rdx, 32			; RDX �� �ִ� ���� 32bit TSC ���� RAX�� �ִ�
	or rax, rdx			; ���� 32bit TSC ���� OR ����. RAX�� 64bit TSC�� ����

	pop rdx
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; �½�ũ ���� ������� �Լ�
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Context�� �����ϰ� �����͸� ��ü�ϴ� ��ũ��
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

	mov ax, ds		; ds, es�� ���ÿ� ���� push�� �Ұ���. ��????????
	push rax
	mov ax, es
	push rax
	push fs
	push gs
%endmacro

; Context ���� ��ũ��
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

; Current Context�� ���� Context�� �����ϰ� Next Task���� Context ����
; 	PARAM : Current Context, Next Context
kSwitchContext:
	push rbp 		; ���ÿ� RBP ����, RSP�� RBP�� ����
	mov rbp, rsp

	; Current Context�� Null�̸� Context�� ������ �ʿ� ����
	pushfq 		; �Ʒ��� cmp ����� RFLAGS�� ������ �ʵ��� ���ÿ� ����
	cmp rdi, 0  ; Current Context�� Null �̸� ���ؽ�Ʈ �������� �ٷ� �̵�
	je LoadContext
	popfq 		; ���ÿ� ������ RFLAGS ����

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; ���� Task �� Context�� ����
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push rax 			; Context ������ offset���� ����� rax�� ���ÿ� ����

	; SS, RSP, RFLAGS, CS, RIP ������ ����
	mov ax, ss 						; SS ����
	mov qword[rdi + (23 * 8)], rax

	mov rax, rbp 					; RBP�� ����� RSP ����
	add rax, 16 					; RSP�� push rbp�� return address��
	mov qword[rdi + (22 * 8)], rax  ; ������ ������ ����

	pushfq							; RFLAGS ����
	pop rax
	mov qword[rdi + (21 * 8)], rax

	mov ax, cs 						; CS ����
	mov qword[rdi + (20 * 8)], rax

	mov rax, qword[rbp + 8] 		; RIP�� return address�� ����
	mov qword[rdi + (19 * 8)], rax 	; ���� ���ؽ�Ʈ ���� �ÿ� �� �Լ��� ȣ���� ��ġ�� �̵�

	; ������ register ���� �� interrupt�� �߻����� ��ó�� ������ context�� ��� ����
	pop rax
	pop rbp

	; ���� �� �κп� SS, RSP, RFLAGS, CS, RIP�� ���������Ƿ�, ���� ������
	; push ������� ���ؽ�Ʈ�� �����ϱ� ���� ������ ����
	add rdi, (19 * 8) 	; rdi�� context �ڷᱸ���� ���� address�� ����ִ�.
	mov rsp, rdi 		; �̸� �������� ����ϱ� ���� 24-5 = 19(��°��ġ ���� ���ۺκ�)
	sub rdi, (19 * 8)

	; ������ �������͸� ��� Context �ڷᱸ���� ����
	KSAVECONTEXT

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; ���� �½�ũ�� ���ؽ�Ʈ ����
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	LoadContext:
		mov rsp, rsi

		; Context �ڷᱸ������ �������͸� ����
		KLOADCONTEXT
		iretq

; ���μ����� ���� ��
; 	PARAM : ����
kHlt:
	hlt 	; ���μ����� ��� ���·� ���Խ�Ŵ
	hlt
	ret
