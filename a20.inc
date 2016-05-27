;*************************************************************
; Enable A20. This is at the very best an annoying procedure.
; A20 code ported from SYSLINUX 1.52-1.63 by H. Peter Anvin.
; AMD Elan bug fix by Robert Schwebel.
;*************************************************************

;*************************************************************
; CPU가 AMD ELAN인 경우
;
; mov al, 0x02
; out 0x92, al
; a20_elan_wait:
; call a20_test
; jz a20_elan_wait
; jmp a20_done
;
;*************************************************************

A20_TEST_LOOPS 		equ 32
A20_ENABLE_LOOPS 	equ 255
A20_TEST_ADDR 		equ 4*0x80 ; (4*0x80 = 0x200)

a20_try_loop:
; first, check the A20 gate on PC
a20_none:
  call a20_test 
  jnz a20_done

; next, test the BIOS (int 0x15, ax=0x2401)
a20_bios:
  mov ax, 0x2401
  pushfd
  int 0x15
  popfd

  call a20_test 
  jnz a20_done

; Turn on A20 gate through keyboard controller.
a20_kbc:
  call empty_8042

  call a20_test 
  jnz a20_done

  mov al, 0xD1
  out 0x64, al 
  call empty_8042

  mov al, 0xDF
  out 0x60, al 
  call empty_8042

; Take the time to turn on the A20 gate.
a20_kbc_wait:
  xor cx, cx

  a20_kbc_wait_loop:
    call a20_test 
    jnz a20_done
    loop a20_kbc_wait_loop

; Last test for 'Control port A'
a20_fast:
  in al, 0x92
  or al, 0x02
  and al, 0xFE
  out 0x92, al 

; Take the time to effect on Control port A
a20_fast_wait:
  xor cx, cx

  a20_fast_wait_loop:
    call a20_test
    jnz a20_done 
    loop a20_fast_wait_loop

; A20 gate doesn't work. Try again.
dec byte [a20_tries]
jnz a20_try_loop

a20_die:
  hlt
  jmp a20_die

a20_tries:
  db A20_ENABLE_LOOPS
  ; A20 gate have turned on this moment.

a20_done: 	; a20 gate's state is on.
  ret 		; a20_try_loop return call.

a20_test:
  push cx
  push ax
  xor cx, cx 		; cx = 0
  mov fs, cx
  dec cx 			; cx = 0xffff (cx => 16bit)
  mov gs, cx 		; 			  (ecx => 32bit)
  mov cx, A20_TEST_LOOPS 	; loop count (=A20_TEST_LOOPS)
  mov ax, word [fs:A20_TEST_ADDR]
  push ax

  a20_test_wait:
    inc ax
    mov word [fs:A20_TEST_ADDR], ax
	  call delay
	  cmp ax, word [gs:A20_TEST_ADDR+0x10]
	  loop a20_test_wait

	  pop word [fs:A20_TEST_ADDR]
	  pop ax
	  pop cx
    ret

empty_8042:
  push ecx
  mov ecx, 100000

  empty_8042_loop:
    dec ecx
    jz empty_8042_end_loop
    call delay
    in al, 0x64
    test al, 1
    jz no_output
    call delay
    in al, 0x60
    jmp empty_8042_loop

no_output:
  test al, 2
  jnz empty_8042_loop

empty_8042_end_loop:
  pop ecx
  ret

delay:
  out 0x80, al 
  ret