[ORG 0x00]
[BITS 16]

SECTION .text

START:
  mov ax, 0x1000

  mov ds, ax
  mov es, ax

  cli
  lgdt[GDTR]

  mov eax, 0x4000003B
  mov cr0, eax

  jmp dword 0x08:(PROTECTEDMODE - $$ + 0x10000)

[BITS 32]
PROTECTEDMODE:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ss, ax
  mov esp, 0xFFFE
  mov ebp, 0xFFFE

  push (switchSuccessString - $$ + 0x10000)
  push 2
  push 0
  call PrintMessage
  add esp, 12

  jmp $

PrintMessage:
  push ebp
  mov ebp, esp

  pushad

  mov eax, dword [ebp+12]
  mov esi, 160
  mul esi
  mov edi, eax

  mov eax, dword [ebp+8]
  mov esi, 2
  mul esi
  add edi, eax

  mov esi, dword [ebp+16]

  MessageLoop:
    mov cl, byte [esi]
    cmp cl, 0
    je MessageEnd

    mov byte [edi + 0xB8000], cl

    add esi, 1
    add edi, 2

    jmp MessageLoop

  MessageEnd:
    popad
    pop ebp
    ret

align 8, db 0
dw 0x0000

GDTR:
  dw GDTEND - GDT - 1
  dd (GDT - $$ + 0x10000)

  GDT:
    NULLDescriptor:
      dd 0x00000000
      dd 0x00000000

    CodeDescriptor:
      dw 0xFFFF
      dw 0x0000
      db 0x00
      db 0x9A
      db 0xCF
      db 0x00

    DataDescriptor:
      dw 0xFFFF
      dw 0x0000
      db 0x00
      db 0x92
      db 0xCF
      db 0x00
  GDTEND:

switchSuccessString: db "Switch To Protected Mode Successful", 0

times 512-($-$$) db 0


