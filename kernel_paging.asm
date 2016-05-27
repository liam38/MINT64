%include "init.inc"
<<<<<<< HEAD
[org 0xC0000000]
[bits 32]

  mov esp, 0xC0000FFF

%include "idt0.inc"

  lidt[idtr]

  mov al, 0xFC
  out 0x21, al
  sti 

  mov ax, TSSSelector
  ltr ax

  mov eax, [CurrentTask]
  add eax, TaskList
  lea edx, [User1regs]
  mov [eax], edx
  add eax, 4

  lea edx, [User2regs]
  mov [eax], edx
  add eax, 4

  lea edx, [User3regs]
  mov [eax], edx
  add eax, 4

  lea edx, [User4regs]
  mov [eax], edx
  add eax, 4
  
  lea edx, [User5regs]
  mov [eax], edx

  mov eax, [CurrentTask]
  add eax, TaskList
  mov ebx, [eax]
  jmp sched

scheduler:
  lea esi, [esp]

  xor eax, eax
  mov eax, [CurrentTask]
  add eax, TaskList

  mov edi, [eax]

  mov ecx, 17
  rep movsd 
  add esp, 68

  add dword [CurrentTask], 4
  mov eax, [NumTask]
  mov ebx, [CurrentTask]
  cmp eax, ebx
  jne yet
  mov byte [CurrentTask], 0

yet:
  xor eax, eax
  mov eax, [CurrentTask]
  add eax, TaskList
  mov ebx, [eax]

sched:
  mov eax, [TSS_ESP0_WHERE]
  mov [eax], esp

  lea esp, [ebx]

  popad
  pop ds 
  pop es
  pop fs
  pop gs

  iretd 

CurrentTask dd 0
NumTask dd 20
TaskList: times 5 dd 0

;******************************************
;**************Subroutines*****************
;******************************************
printf:
  push eax
  push es
  mov ax, VideoSelector
  mov es, ax

  printf_loop:
    mov al, byte [esi]
    mov byte [es:edi], al
    inc edi
    mov byte [es:edi], 0x06
    inc esi
    inc edi
    or al, al
    jz printf_end
    jmp printf_loop

printf_end:
  pop es
  pop eax
  ret

;********************************************
;***************Task Structures**************
;********************************************

%include "user_task_structure.inc"

;********************************************
;*********Interrupt Service Routines*********
;********************************************

%include "idt1.inc"

;********************************************
;*********************IDT********************
;********************************************

idtr:
  dw 256*8-1
  dd IDT_BASE

%include "idt2.inc"

times 512*7-($-$$) db 0
=======

[org 0xC0000000]
[bits 32]

	mov esp, 0xC0000FFF

%include "idt0.inc"

	lidt [idtr]		; IDT�� ����Ѵ�.

	mov al, 0xFC            ; ���Ƶξ��� ���ͷ�Ʈ ��, 
	out 0x21, al		; Ÿ�̸ӿ� Ű���常 �ٽ� ��ȿ�ϰ� �Ѵ�.
	sti

	mov ax, TSSSelector
	ltr ax

	mov eax, [CurrentTask]  ; Task Struct�� ����Ʈ�� �����.
	add eax, TaskList
	lea edx, [User1regs]
	mov [eax], edx  
	add eax, 4
	lea edx, [User2regs]
	mov [eax], edx	
	add eax, 4
	lea edx, [User3regs]
	mov [eax], edx	
	add eax, 4
	lea edx, [User4regs]
	mov [eax], edx	
	add eax, 4
	lea edx, [User5regs]
	mov [eax], edx	

	mov eax, [CurrentTask]  ; ù ��° Task�� �����Ѵ�. (CurrentTask = 0)
	add eax, TaskList
	mov ebx, [eax]
	jmp sched

scheduler:
	lea esi,[esp]           ; Ŀ�� ESP���� ���� �������͵��� �ִ�.

	xor eax, eax	
	mov eax, [CurrentTask]
	add eax, TaskList

	mov edi,[eax]           ; ���� ���� ���� �½�ũ�� ���念���� �����Ѵ�.

	mov ecx,17              ; 17���� DWORD(68 BYTE) ��� ���������� ����Ʈ ��.
	rep movsd               ; �����ϰ�, 
	add esp,68              ; 17���� DWORD ��ŭ ������ �ǵ��� ���´�.

	add dword [CurrentTask], 4	
	mov eax, [NumTask]
	mov ebx, [CurrentTask]
	cmp eax, ebx
	jne yet
	mov byte [CurrentTask], 0
yet:
	xor eax, eax
	mov eax, [CurrentTask]
	add eax, TaskList
	mov ebx, [eax]
sched:

        mov eax, [TSS_ESP0_WHERE]
        mov [eax], esp          ; Ŀ�ο����� �����ּҸ� TSS�� ������ �д�.
	                        

	lea esp,[ebx]           ; EBX���� ���� �½�ũ�� ���念���� �ּҰ� �ִ�.

	popad                   ; EAX, EBX, ECX, EDX, EBP, ESI, EDI �� �����Ѵ�.
	pop ds                  ; DS, ES, FS, GS �����Ѵ�.
	pop es
	pop fs
	pop gs

	iretd                   ; ���� ���� �½�ũ�� ����Ī �ȴ�.

CurrentTask dd 0                ; ���� ���� ���� �½�ũ ��ȣ
NumTask dd 20                   ; ��� �½�ũ�� ��
TaskList: times 5 dd 0          ; �� �½�ũ ���念���� ������ �迭


;**************************************
;*********   Subrutines   *************
;**************************************
printf:
	push eax		
	push es
	mov ax, VideoSelector
	mov es, ax
printf_loop:
	mov al, byte [esi]	
	mov byte [es:edi], al	
	inc edi	
	mov byte [es:edi], 0x06 
	inc esi		
	inc edi	
	or al, al	
	jz printf_end
	jmp printf_loop		
printf_end:
	pop es
	pop eax		
	ret


;**************************************
;*******   Task Structures   **********
;**************************************

%include "user_task_structure.inc"

;****************************************
;****   Interrupt Service Rutines  ******
;****************************************

%include "idt1.inc"

;****************************************
;*************   IDT   ******************
;****************************************

idtr:	dw	256*8-1		; IDT �� Limit 
	dd	IDT_BASE        ; IDT �� Base Address

%include "idt2.inc"

times 512*7-($-$$) db 0

>>>>>>> prac7
