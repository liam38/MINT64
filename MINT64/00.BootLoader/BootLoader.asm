[ORG 0x00]
[BITS 16]

SECTION .text

  jmp 0x07C0:START
  totalSectorCount: 	dw 0x02
  kernel32SectorCount:	dw 0x02

START:
  mov ax, 0x07C0
  mov ds, ax

  mov ax, 0x0000
  mov ss, ax
  mov sp, 0xFFFE
  mov bp, 0xFFFE

;============display reset & welcome string================
  mov ax, 0xB800
  mov es, ax
  mov si, 0

  SCREEN_CLEAR_LOOP:
    mov byte [es:si], 0
    mov byte [es:si+1], 0x0A

    add si, 2

    cmp si, 80*25*2
    jl SCREEN_CLEAR_LOOP

  push MESSAGE1
  push 0x00
  push 0x00
  call PrintMessage
  add sp, 6

;===========disk reset==========
  push imageLoadingString
  push 1
  push 0
  call PrintMessage
  add sp, 6

  ResetDisk:
    mov ax, 0
    mov dl, 0
    int 0x13
    jc ResetError

;===========disk load============
  mov si, 0x1000
  mov es, si
  mov bx, 0x0000

  mov di, word [totalSectorCount]

  ReadData:
    cmp di, 0
    je ReadEnd
    sub di, 0x1

    mov ah, 0x02
    mov al, 1
    mov ch, byte [trackNumber]
    mov cl, byte [sectorNumber]
    mov dh, byte [headNumber]
    mov dl, 0x00
    int 0x13
    jc HandleDiskError

    add si, 0x0020
    mov es, si

    mov al, byte [sectorNumber]
    add al, 0x01

    mov byte [sectorNumber], al
    cmp al, 19
    jl ReadData

    xor byte [headNumber], 0x01
    mov byte [sectorNumber], 0x01

    cmp byte [headNumber], 0x00
    jne ReadData

    add byte [trackNumber], 0x01
    jmp ReadData
  ReadEnd:
	push loadingCompleteString
	push 1
	push 20
	call PrintMessage
	add sp, 6

	jmp 0x1000:0000

  ResetError:
    push diskResetError
    push 1
    push 20
    call PrintMessage
    add sp, 6

  HandleDiskError:
    push diskErrorString
    push 1
    push 20
    call PrintMessage
    add sp, 6

  	jmp $

;==============Print Message=================
PrintMessage: 	; PARAM ) x, y, String addr.
  push bp
  mov bp, sp

  pushad

  mov ax, 0xB800
  mov es, ax

  mov ax, word [bp+6]
  mov si, 160
  mul si
  mov di, ax

  mov ax, word [bp+4]
  mov si, 2
  mul si
  add di, ax

  mov si, word [bp+8]

  MessageLoop:
    mov cl, byte [si]
    cmp cl, 0
    je MessageEnd

    mov byte [es:di], cl

    add si, 1
    add di, 2

    jmp MessageLoop

  MessageEnd:
    popad
    pop bp
    ret


sectorNumber: 		db 0x02
headNumber:			db 0x00
trackNumber:		db 0x00

MESSAGE1: 			db "MINT64 OS Boot Loader Start", 0

diskResetError:		db "Reset Error!!", 0
diskErrorString: 	db "DISK Error!!", 0
imageLoadingString:	db "OS Image Loading...", 0
loadingCompleteString:	db "Complete Loading!", 0

times 510 - ($-$$) db 0x00
dw 0xAA55
