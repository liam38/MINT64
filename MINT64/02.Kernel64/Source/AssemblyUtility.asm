[BITS 64]

SECTION .text

;C언어에서 호출할 수 있도록 이름 노출
global kInPortByte, kOutPortByte

; port로부터 1byte 읽음
;  PARAM: port No.
kInPortByte:
	push rdx		; 함수에서 임시로 사용하는 레지스터를 스택에 저장
					; 함수의 마지막 부분에서 스택에 삽입된 값을 꺼내 복원

	mov rdx, rdi	; RDX = 파라미터 1(포트번호)
	mov rax, 0 		; RAX 초기화
	in al, dx 		; dx에 저장된 포트 addr.에서 한 byte를 읽어
					; al에 저장, al은 함수의 반환 값으로 사용

	pop rdx			; 함수에서 사용이 끝난 reg 복원
	ret

; port에 1byte 씀
;  PARAM: port No, data
kOutPortByte:
	push rdx 		; 함수에서 임시로 사용하는 reg를 스택에 저장
	push rax 		; 함수의 마지막 부분에서 스택에 삽입된 값을 꺼내 복원

	mov rdx, rdi 	; RDX = 파라미터 1(포트번호)
	mov rax, rsi 	; RAX = 파라미터 2(데이터)
	out dx, al 		; dx에 저장된 port addr에 al에 저장된 한 바이트 write

	pop rax 		; 함수에서 사용이 끝난 reg 복원
	pop rdx
	ret
