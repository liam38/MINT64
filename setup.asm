%include "init.inc"

PAGE_DIR        equ 0x100000
PAGE_TAB_KERNEL equ 0x101000
PAGE_TAB_USER   equ 0x102000
PAGE_TAB_LOW    equ 0x103000

[org 0x90000]
[bits 16]
start:
	cld			
	mov	ax,cs
	mov	ds,ax
	xor	ax,ax
	mov	ss,ax

	xor eax, eax
   	lea eax,[tss]          ; EAX�� tss�� �����ּҸ� �ִ´�.
	add eax, 0x90000
    	mov [descriptor4+2],ax
    	shr eax,16
   	mov [descriptor4+4],al
    	mov [descriptor4+7],ah

	cli
        lgdt[gdtr]

        mov eax, cr0
        or eax, 0x00000001
        mov cr0, eax

        jmp $+2
 	nop
	nop

        jmp dword SysCodeSelector:PM_Start

[bits 32]

PM_Start:
	
	mov bx, SysDataSelector
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	mov ss, bx

	lea esp, [PM_Start]


        mov esi, 0x80000              ; Ŀ���� �����ּ� 0x200000 �� �ű��.
        mov edi, 0x200000
        mov cx, 512*7
kernel_copy:
	mov al, byte [ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	dec cx
	jnz kernel_copy


        mov esi, 0x70000              ; user_program1.bin �� 0x300000�� �ű��.
        mov edi, 0x300000
        mov cx, 512
user1_copy:
	mov al, byte [ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	dec cx
	jnz user1_copy


        mov esi, 0x70200              ; user_program2.bin �� 0x301000�� �ű��.
        mov edi, 0x301000
        mov cx, 512
user2_copy:
	mov al, byte [ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	dec cx
	jnz user2_copy

        mov esi, 0x70400              ; user_program3.bin �� 0x302000�� �ű��.
        mov edi, 0x302000
        mov cx, 512
user3_copy:
	mov al, byte [ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	dec cx
	jnz user3_copy

        mov esi, 0x70600              ; user_program4.bin �� 0x303000�� �ű��.
        mov edi, 0x303000
        mov cx, 512
user4_copy:
	mov al, byte [ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	dec cx
	jnz user4_copy

        mov esi, 0x70800              ; user_program5.bin �� 0x304000�� �ű��.
        mov edi, 0x304000
        mov cx, 512
user5_copy:
	mov al, byte [ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	dec cx
	jnz user5_copy

	        	           ; ������ ���丮�� not present �� �ʱ�ȭ �Ѵ�.
	mov edi, PAGE_DIR          ; PAGE_DIR = 0x100000  
	mov eax, 0                 ; not present
	mov ecx, 1024              ; ������ ����
	cld
	rep stosd

        mov edi, PAGE_DIR          ; ������ ���丮�� 
        mov eax, 0x103000          ; 0��° ��Ʈ�� - 0x103000�� ������ ���̺�.
        or  eax, 0x01
        mov [es:edi], eax        
	
	mov edi, PAGE_DIR+0x200*4  ; 0x80000000 �� ���� 10��Ʈ*4
	mov eax, 0x102000          ; 0x200 ��°�� ��Ʈ�� - 0x102000�� ���̺�.
	or eax, 0x07               ; ���� �������� ǥ��
	mov [es:edi], eax

	mov edi, PAGE_DIR+0x300*4  ; 0xC0000000 �� ���� 10��Ʈ*4
        mov eax, 0x101000          ; 0x300 ��°�� ��Ʈ�� - 0x101000�� ���̺�.
        or eax, 0x01               ; Ŀ�� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_KERNEL   ; 0x101000 ~ 0x101FFF ���� �ʱ�ȭ �Ѵ�.
	mov eax, 0                 ; not present
	mov ecx, 1024              ; ������ ����
	cld
	rep stosd	

        mov edi, PAGE_TAB_KERNEL+0x000*4
        mov eax, 0x200000          ; 0��° ��Ʈ�� - 0x200000�� ����������.
        or  eax, 1                 ; Ŀ�� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_KERNEL+0x001*4
        mov eax, 0x201000          ; 1��° ��Ʈ�� - 0x201000�� ����������. 
        or  eax, 1                 ; Ŀ�� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_USER     ; 0x102000 ~ 0x102FFF ���� �ʱ�ȭ �Ѵ�.
	mov eax, 0x00              ; not present
	mov ecx, 1024              ; ������ ����
	cld
	rep stosd	

        mov edi, PAGE_TAB_USER+0x000*4
        mov eax, 0x300000          ; 0��° ��Ʈ�� - 0x300000�� ����������.
        or  eax, 0x07              ; ���� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_USER+0x001*4
        mov eax, 0x301000          ; 1��° ��Ʈ�� - 0x301000�� ����������.
        or  eax, 0x07              ; ���� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_USER+0x002*4
        mov eax, 0x302000          ; 2��° ��Ʈ�� - 0x302000�� ����������.
        or  eax, 0x07              ; ���� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_USER+0x003*4
        mov eax, 0x303000          ; 3��° ��Ʈ�� - 0x303000�� ����������.
        or  eax, 0x07              ; ���� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_USER+0x004*4
        mov eax, 0x304000          ; 4��° ��Ʈ�� - 0x304000�� ����������.
        or  eax, 0x07              ; ���� �������� ǥ��
        mov [es:edi], eax

        mov edi, PAGE_TAB_LOW      ; 1MB ������ ������ ����(0 ~ 0xFFFFF)
        mov eax, 0x00000
        or  eax, 0x01              ; Ŀ�� �������� ǥ��
        mov cx, 256                ; 256���� ��Ʈ�� (1/4�� ����)
page_low_loop:
        mov [es:edi], eax
        add eax, 0x1000            ; 0x1000 ������ �����ּҸ� ������.
	add edi, 4
        dec cx
        jnz page_low_loop

        lea eax, [tss_esp0]
        mov [TSS_ESP0_WHERE], eax 

	mov eax, PAGE_DIR
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

        jmp 0xC0000000             ; kernel.bin���� �����Ѵ�.


;***************************************
;**********   Data Area   **************
;***************************************
gdtr:	dw	gdt_end-gdt-1
	dd	gdt
gdt:
	dd 0, 0
	dd 0x0000FFFF, 0x00CF9A00
	dd 0x0000FFFF, 0x00CF9200
	dd 0x8000FFFF, 0x0040920B

descriptor4:				;TSS ��ũ����
	dw 104
	dw 0
	db 0
	db 0x89
	db 0
	db 0

	dd	0x0000FFFF, 0x00FCFA00  ;���� �ڵ� ���׸�Ʈ
	dd	0x0000FFFF, 0x00FCF200  ;���� ������ ���׸�Ʈ
gdt_end:




tss: 
   	 dw 0, 0                ; ���� �½�ũ���� back link
tss_esp0:
    	dd 0                    ; ESP0
        dw SysDataSelector, 0   ; SS0, ������
        dd 0                    ; ESP1
        dw 0, 0                 ; SS1, ������
   	dd 0                    ; ESP2
    	dw 0, 0                 ; SS2, ������
   	dd 0x100000
tss_eip:
        dd 0, 0                 ; EIP, EFLAGS
        dd 0, 0, 0, 0
tss_esp:
        dd 0, 0, 0, 0           ; ESP, EBP, ESI, EDI
        dw 0, 0                 ; ES, ������
        dw 0, 0                 ; CS, ������
        dw UserDataSelector, 0 	; SS, ������
        dw 0, 0                 ; DS, ������
        dw 0, 0                 ; FS, ������
        dw 0, 0                 ; GS, ������
        dw 0, 0                 ; LDT, ������
        dw 0, 0                 ; ����׿� T��Ʈ, IO �㰡 ��Ʈ��


times 1024-($-$$) db 0

