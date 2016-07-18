[ORG 0x00]
[BITS 16]

SECTION .text

START:
  mov ax, 0x1000

  mov ds, ax
  mov es, ax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; A20 Gate를 활성화
; BIOS를 통한 전환이 실패시, 시스템 컨트롤 포트로 전환 시도
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  mov ax, 0x2401
  int 0x15

  jc A20GateError
  jmp A20GateSuccess

A20GateError:
  in al, 0x92
  or al, 0x02
  and al, 0xFE
  out 0x92, al

A20GateSuccess:
  cli
  lgdt[GDTR]

  mov eax, 0x4000003B
  mov cr0, eax

  jmp dword 0x18:(PROTECTEDMODE - $$ + 0x10000)

[BITS 32]
PROTECTEDMODE:
  mov ax, 0x20
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

  jmp dword 0x18:0x10200

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

	IA_32eCodeDescriptor:
	  dw 0xFFFF
	  dw 0x0000
	  db 0x00
	  db 0x9A
	  db 0xAF
	  db 0x00

	IA_32eDataDescriptor:
	  dw 0xFFFF
	  dw 0x0000
	  db 0x00
	  db 0x92
	  db 0xAF
	  db 0x00

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


