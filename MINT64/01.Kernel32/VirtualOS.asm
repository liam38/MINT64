[ORG 0x00]
[BITS 16]

SECTION .text

  jmp 0x1000:START

sectorCount: 		dw 0x0000
totalSectorCount	equ 1024

START:
  mov ax, cs
  mov ds, ax
  mov ax, 0xB800
  mov es, ax

  %assign index 0
  %rep totalSectorCount
    %assign index index+1

	mov ax, 2
	mul word [sectorCount]
	mov si, ax
	mov byte [es:si + (160*2)], '0' + (index % 10)
	add word [sectorCount], 1

	%if index == totalSectorCount
	  jmp $

	%else
	  jmp (0x1000 + index*0x20):0x0000

	%endif

	times (512 - ($-$$) % 512) db 0
  %endrep
