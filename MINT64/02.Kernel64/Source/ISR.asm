[BITS 64]

SECTION .text

extern kCommonExceptionHandler, kCommonInterruptHandler, kKeyboardHandler
extern kTimerHandler, kDeviceNotAvailableHandler, kHDDHandler

; Exceipton ISR
global kISRDivideError, kISRDebug, kISRNMI, kISRBreakPoint, kISROverflow
global kISRBoundRangeExceeded, kISRInvalidOpcode, kISRDeviceNotAvailable, kISRDoubleFault
global kISRCoprocessorSegmentOverrun, kISRInvalidTSS, kISRSegmentNotPresent
global kISRStackSegmentFault, kISRGeneralProtection, kISRPageFault, kISR15
global kISRFPUError, kISRAlignmentCheck, kISRMachineCheck, kISRSIMDError, kISRETCException

; Interrupt ISR
global kISRTimer, kISRKeyboard, kISRSlavePIC, kISRSerial2, kISRSerial1, kISRParallel2
global kISRFloppy, kISRParallel1, kISRRTC, kISRReserved, kISRNotUsed1, kISRNotUsed2
global kISRMouse, kISRCoprocessor, kISRHDD1, kISRHDD2, kISRETCInterrupt

; Context save, selector change macro
%macro KSAVECONTEXT 0 		; no PARAM
	; insert in stack, Between RBP reg to GS Segment selector
	push rbp
	mov rbp, rsp
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov ax, ds			; DS, ES 셀렉터는 스택에 직접 삽입 불가
	push rax			; 따라서 RAX에 저장한 후 스택에 삽입
	mov ax, es
	push rax
	push fs
	push gs

	; change segment selector
	mov ax, 0x10 		; ax에 kernel data segment descriptor 저장
	mov ds, ax			; ds부터 fs까지 모두 kerenel data segment로 교체
	mov es, ax
	mov gs, ax
	mov fs, ax
%endmacro

; Context Load macro
%macro KLOADCONTEXT 0
	;gs 부터 rbp까지 복원
	pop gs
	pop fs
	pop rax
	mov es, ax
	pop rax
	mov ds, ax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop rbp
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	Exception Handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; #0, Divide Error ISR
kISRDivideError:
	KSAVECONTEXT

	mov rdi, 0 	; 64bit(IA-32e mode)에서 parameter : Interrupt Vector value.
	call kCommonExceptionHandler 	; handler 종료시 해당 interrupt에 EOI를 보내주기 위함.

	KLOADCONTEXT
	iretq

; #1, Debug ISR
kISRDebug:
	KSAVECONTEXT

	mov rdi, 1
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #2, NMI ISR
kISRNMI:
	KSAVECONTEXT

	mov rdi, 2
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #3, Break Point ISR
kISRBreakPoint:
	KSAVECONTEXT

	mov rdi, 3
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #4, Over flow ISR
kISROverflow:
	KSAVECONTEXT

	mov rdi, 4
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #5, Bound Range Exceed ISR
kISRBoundRangeExceeded:
	KSAVECONTEXT

	mov rdi, 5
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #6, Invalid Opcode ISR
kISRInvalidOpcode:
	KSAVECONTEXT

	mov rdi, 6
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #7, Device Not Available ISR
kISRDeviceNotAvailable:
	KSAVECONTEXT

	mov rdi, 7
	call kDeviceNotAvailableHandler

	KLOADCONTEXT
	iretq

; #8, Double Fault ISR
kISRDoubleFault:
	KSAVECONTEXT

	mov rdi, 8
	mov rsi, qword [rbp + 8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #9, Coprocessor Segment Overrun ISR
kISRCoprocessorSegmentOverrun:
	KSAVECONTEXT

	mov rdi, 9
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #10, Invalid TSS ISR
kISRInvalidTSS:
	KSAVECONTEXT

	mov rdi, 10
	mov rsi, qword [rbp + 8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #11, Segment Not Present ISR
kISRSegmentNotPresent:
	KSAVECONTEXT

	mov rdi, 11
	mov rsi, qword [rbp + 8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #12, Stack Segmnet Fault ISR
kISRStackSegmentFault:
	KSAVECONTEXT

	mov rdi, 12
	mov rsi, qword [rbp + 8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #13, General Protection ISR
kISRGeneralProtection:
	KSAVECONTEXT

	mov rdi, 13
	mov rsi, qword [rbp + 8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #14, Page Fault ISR
kISRPageFault:
	KSAVECONTEXT

	mov rdi, 14
	mov rsi, qword [rbp + 8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #15, 15 ISR
kISR15:
	KSAVECONTEXT

	mov rdi, 15
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #16, FPU Error ISR
kISRFPUError:
	KSAVECONTEXT

	mov rdi, 16
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #17, Alignment Check ISR
kISRAlignmentCheck:
	KSAVECONTEXT

	mov rdi, 17
	mov rsi, qword [rbp + 8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #18, Machine Check ISR
kISRMachineCheck:
	KSAVECONTEXT

	mov rdi, 18
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #19, SIMD Floating Point Exception ISR
kISRSIMDError:
	KSAVECONTEXT

	mov rdi, 19
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #20~#31, Reserved ISR
kISRETCException:
	KSAVECONTEXT

	mov rdi, 20
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	Interrupt Handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; #32, Timer ISR
kISRTimer:
	KSAVECONTEXT

	mov rdi, 32
	call kTimerHandler

	KLOADCONTEXT
	iretq

; #33, Keyboard ISR
kISRKeyboard:
	KSAVECONTEXT

	mov rdi, 33
	call kKeyboardHandler

	KLOADCONTEXT
	iretq

; #34, Slave PIC ISR
kISRSlavePIC:
	KSAVECONTEXT

	mov rdi, 34
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #35, Serial port2 ISR
kISRSerial2:
	KSAVECONTEXT

	mov rdi, 35
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #36, Serial port1 ISR
kISRSerial1:
	KSAVECONTEXT

	mov rdi, 36
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #37, Parallel port2 ISR
kISRParallel2:
	KSAVECONTEXT

	mov rdi, 37
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #38, Floppy disk controller ISR
kISRFloppy:
	KSAVECONTEXT

	mov rdi, 38
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #39, Parallel port1 ISR
kISRParallel1:
	KSAVECONTEXT

	mov rdi, 39
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #40, RTC ISR
kISRRTC:
	KSAVECONTEXT

	mov rdi, 40
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #41, Reserved interrupt ISR
kISRReserved:
	KSAVECONTEXT

	mov rdi, 41
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #42, Not Used ISR
kISRNotUsed1:
	KSAVECONTEXT

	mov rdi, 42
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #43, Not Used ISR
kISRNotUsed2:
	KSAVECONTEXT

	mov rdi, 43
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #44, Mouse ISR
kISRMouse:
	KSAVECONTEXT

	mov rdi, 44
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #45, Coprocessor ISR
kISRCoprocessor:
	KSAVECONTEXT

	mov rdi, 45
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq

; #46, HDD 1 ISR
kISRHDD1:
	KSAVECONTEXT

	mov rdi, 46
	call kHDDHandler

	KLOADCONTEXT
	iretq

; #47, HDD 2 ISR
kISRHDD2:
	KSAVECONTEXT

	mov rdi, 47
	call kHDDHandler

	KLOADCONTEXT
	iretq

; #48 이외의 all interrupt ISR
kISRETCInterrupt:
	KSAVECONTEXT

	mov rdi, 48
	call kCommonInterruptHandler

	KLOADCONTEXT
	iretq
