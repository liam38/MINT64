[BITS 64]

SECTION .text

;�ܺο��� ���ǵ� �Լ��� �� �� �ֵ��� ������ (Import)
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
