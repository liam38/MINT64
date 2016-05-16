%include "init.inc"
[org 0x10000]
[bits 16]

start:
  cld
  mov ax, cs
  mov ds, ax
  xor ax, ax
  mov ss, ax

  xor ebx, ebx
  lea eax, [tss1]
  add eax, 0x10000
  mov [descriptor4+2], ax
  shr eax, 16
  mov [descriptor4+4], al
  mov [descriptor4+7], ah

  lea eax, [tss2]
  add eax, 0x10000
  mov [descriptor5+2], ax
  shr eax, 16
  mov [descriptor5+4], al
  mov [descriptor5+7], ah

  cli

  lgdt [gdtr]

  mov eax, cr0
  or eax, 0x1
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

  mov ax, TSS1Selector
  ltr ax                    ; LTR ) CPU TR reg.에 TSS descriptor의 selector 값 저장
  lea eax, [process2]       ; TSS1Selector ) task switching에 의해 저장되어야 할 곳.
  mov [tss2_eip], eax       ; eip에 저장된 주소로부터 다음 명령어 실행
  mov [tss2_esp], esp       ; 스택 포인터

  jmp TSS2Selector:0        ; 실제 task switching 이 일어나는 곳.

  mov edi, 80*2*9
  lea esi, [msg_process1]
  call printf
  jmp $

;============Subroutines============
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

process2:                 ; jmp TSS2Selector:0 으로 인해 실행되는 지점
  mov edi, 80*2*7         ; eip = process2의 주소
  lea esi, [msg_process2]
  call printf
  jmp TSS1Selector:0      ;tss1 에 저장되어 있는 값들 복귀(eip 역시)

;=============Data Area=============
msg_process1 db "This is System Process 1", 0
msg_process2 db "This is System Process 2", 0

gdtr:
  dw gdt_end - gdt - 1
  dd gdt

  gdt:
    dd 0, 0
    dd 0x0000FFFF, 0x00CF9A00
    dd 0x0000FFFF, 0x00CF9200
    dd 0x8000FFFF, 0x0040920B

    descriptor4:              ;TSS1 descriptor(8byte = 64bits)
      dw 104
      dw 0
      db 0
      db 0x89
      db 0
      db 0

    descriptor5:              ;TSS2 descriptor (8byte = 64bits)
      dw 104
      dw 0
      db 0
      db 0x89
      db 0
      db 0

gdt_end:

tss1:                         ; TSS1Selector 의 base가 가리키는 위치
  dw 0, 0                     ; 실질적인 tss1 영역
  dd 0
  dw 0, 0
  dd 0
  dw 0, 0
  dd 0
  dw 0, 0
  dd 0, 0, 0
  dd 0, 0, 0, 0
  dd 0, 0, 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0

tss2:
  dw 0, 0
  dd 0
  dw 0, 0
  dd 0
  dw 0, 0
  dd 0
  dw 0, 0
  dd 0

tss2_eip:
  dd 0, 0
  dd 0, 0, 0, 0

tss2_esp:
  dd 0, 0, 0, 0
  dw SysDataSelector, 0
  dw SysCodeSelector, 0
  dw SysDataSelector, 0
  dw SysDataSelector, 0
  dw SysDataSelector, 0
  dw SysDataSelector, 0
  dw 0, 0
  dw 0, 0

times 1024-($-$$) db 0