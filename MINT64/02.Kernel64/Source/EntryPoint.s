[BITS 64]

SECTION .text

;외부에서 정의된 함수를 쓸 수 있도록 선언함 (Import)
extern Main

START:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ;Create Stack between 0x600000 and 0x6FFFFF.
  mov ss, ax
  mov rsp, 0x6FFFF8
  mov rbp, 0x6FFFF8

  call Main

  jmp $
