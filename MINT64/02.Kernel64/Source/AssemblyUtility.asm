[BITS 64]

SECTION .text

;C���� ȣ���� �� �ֵ��� �̸� ����
global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS
global kReadTSC

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
