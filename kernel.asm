%include "init.inc"
[org 0x10000]
[bits 16]

start:
  cld 
  mov ax, cs
  mov ds, ax
  xor ax, ax
  mov ss, ax

  xor eax, eax
  lea eax, [tss]
  add eax, 0x10000
  mov [descriptor4+2], ax
  shr eax, 16
  mov [descriptor4+4], al
  mov [descriptor4+7], ah

  xor eax, eax
  lea eax, [printf]
  add eax, 0x10000
  mov [descriptor7], ax
  shr eax, 16
  mov [descriptor7+6], al
  mov [descriptor7+7], ah

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

  mov ax, TSSSelector
  ltr ax

  mov [tss_esp0], esp
  lea eax, [PM_Start-256]
  mov [tss_esp], eax

  mov ax, UserDataSelector
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  lea esp, [PM_Start-256]

  push dword UserDataSelector
  push esp
  push dword 0x200
  push dword UserCodeSelector
  lea eax, [user_process]
  push eax
  iretd

; =============Subroutines============
printf:
  mov ebp, esp
  push es
  push eax
  mov ax, VideoSelector
  mov es, ax
  mov esi, [ebp+8]
  mov edi, [ebp+12]

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
  pop eax
  pop es
  ret

user_process:
  mov edi, 80*2*7
  push edi
  lea eax, [msg_user_parameter1]
  push eax
  call 0x38:0
  jmp $

msg_user_parameter1 db "This is User Parameter 1", 0

;============Data Area=============
gdtr:
  dw gdt_end - gdt - 1
  dd gdt 

  gdt:
    dd 0, 0
    dd 0x0000FFFF, 0x00CF9A00
    dd 0x0000FFFF, 0x00CF9200
    dd 0x8000FFFF, 0x0040920B

    descriptor4:
      dw 104
      dw 0
      db 0
      db 0x89
      db 0
      db 0

      dd 0x0000FFFF, 0x00FCFA00
      dd 0x0000FFFF, 0x00FCF200

    descriptor7:
      dw 0
      dw SysCodeSelector
      db 0x02
      db 0xEC
      db 0
      db 0

  gdt_end:

tss:
  dw 0, 0

tss_esp0:
  dd 0
  dw SysDataSelector, 0
  dd 0
  dw 0, 0
  dd 0
  dw 0, 0
  dd 0

tss_eip:
  dd 0, 0
  dd 0, 0, 0, 0

tss_esp:
  dd 0, 0, 0, 0
  dw 0, 0
  dw 0, 0
  dw UserDataSelector, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0
  dw 0, 0

times 1024-($-$$) db 0