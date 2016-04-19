%include "init.inc"
[org 0]
  jmp 0x07C0:start

start:
  mov ax, cs
  mov ds, ax
  mov es, ax

  mov ax, 0xB800
  mov es, ax
  mov di, 0
  mov ax, word [msgBack]
  mov cx, 0x7FF

paint:
  mov word [es:di], ax
  add di, 2
  dec cx
  jnz paint

read:
  mov ax, 0x1000
  mov es, ax
  mov bx, 0

  mov ah, 2
  mov al, 2 
  mov ch, 0
  mov cl, 2
  mov dh, 0
  mov dl, 0
  int 0x13

  jc read

  mov dx, 0x3F2
  xor al, al
  out dx, al

  cli 

  mov al, 0x11
  out 0x20, al
  dw 0x00eb, 0x00eb
  out 0xA0, al
  dw 0x00eb, 0x00eb

  mov al, 0x20
  out 0x21, al
  dw 0x00eb, 0x00eb
  mov al, 0x28
  out 0xA1, al
  dw 0x00eb, 0x00eb

  mov al, 0x04
  out 0x21, al
  dw 0x00eb, 0x00eb
  mov al, 0x02
  out 0xA1, al
  dw 0x00eb, 0x00eb

  mov al, 0x01
  out 0x21, al
  dw 0x00eb, 0x00eb
  out 0xA1, al
  dw 0x00eb, 0x00eb

  mov al, 0xFF
  out 0xA1, al
  dw 0x00eb, 0x00eb
  mov al, 0xFB
  out 0x21, al

  jmp 0x1000:0000

msgBack db '.', 0x0A 

times 510-($-$$) db 0
dw 0xAA55
