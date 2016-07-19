[BITS 64]

SECTION .text

;C���� ȣ���� �� �ֵ��� �̸� ����
global kInPortByte, kOutPortByte

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
