%include "init.inc"
<<<<<<< HEAD
[org 0]
  jmp 0x07C0:start
=======

[org 0]
            jmp 07C0h:start     
>>>>>>> prac7

%include "a20.inc"

start:
<<<<<<< HEAD
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
  mov ax, 0x9000
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

read_kernel:
  mov ax, 0x8000
  mov es, ax
  mov bx, 0

  mov ah, 2
  mov al, 7
  mov ch, 0
  mov cl, 4
  mov dh, 0
  mov dl, 0
  int 0x13

  jc read_kernel

read_user1:
  mov ax, 0x7000
  mov es, ax
  mov bx, 0

  mov ah, 2
  mov al, 1
  mov ch, 0
  mov cl, 11
  mov dh, 0
  mov dl, 0
  int 0x13

  jc read_user1

read_user2:
  mov ax, 0x7000
  mov es, ax
  mov bx, 0x200

  mov ah, 2
  mov al, 1
  mov ch, 0
  mov cl, 12
  mov dh, 0
  mov dl, 0
  int 0x13

  jc read_user2

read_user3:
  mov ax, 0x7000
  mov es, ax
  mov bx, 0x400

  mov ah, 2
  mov al, 1
  mov ch, 0
  mov cl, 13
  mov dh, 0
  mov dl, 0
  int 0x13

  jc read_user3

read_user4:
  mov ax, 0x7000
  mov es, ax
  mov bx, 0x600

  mov ah, 2
  mov al, 1
  mov ch, 0
  mov cl, 14
  mov dh, 0
  mov dl, 0
  int 0x13

  jc read_user4

read_user5:
  mov ax, 0x7000
  mov es, ax
  mov bx, 0x800

  mov ah, 2
  mov al, 1
  mov ch, 0
  mov cl, 15
  mov dh, 0
  mov dl, 0
  int 0x13

  jc read_user5

  mov dx, 0x3F2
  xor al, al 
  out dx, al 

  cli

  call a20_try_loop

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

  mov al, 0xff
  out 0xA1, al 
  dw 0x00eb, 0x00eb 
  mov al, 0xFB
  out 0x21, al 

  jmp 0x9000:0000 		; jmp to setup.bin

msgBack db '.', 0x67 

times 510-($-$$) db 0
dw 0xAA55
=======
            
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
	    add di,2		
	    dec cx		    
	    jnz paint	   	

read:
            mov ax, 0x9000      ; ES:BX = 9000:0000
            mov es, ax          ;
            mov bx, 0           ;

            mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로  
            mov al, 2           ; 2 섹터를 읽을 것이다.
            mov ch, 0           ; 0번째 Cylinder
            mov cl, 2           ; 1번째 섹터부터 읽기 시작한다. 
            mov dh, 0           ; Head=0
            mov dl, 0           ; Drive=0  A:드라이브
            int 13h             ; Read!

            jc read             ; 에러가 나면, 다시 함. 


read_kernel:
            mov ax, 0x8000      ; ES:BX = 8000:0000
            mov es, ax          ;
            mov bx, 0           ;

            mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로  
            mov al, 7           ; 7 섹터를 읽을 것이다.
            mov ch, 0           ; 0번째 Cylinder
            mov cl, 4           ; 4번째 섹터부터 읽기 시작한다. 
            mov dh, 0           ; Head=0
            mov dl, 0           ; Drive=0  A:드라이브
            int 13h             ; Read!

            jc read_kernel      ; 에러가 나면, 다시 함. 

read_user1:
            mov ax, 0x7000      ; ES:BX = 7000:0000
            mov es, ax          ;
            mov bx, 0           ;

            mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로  
            mov al, 1           ; 1 섹터를 읽을 것이다.
            mov ch, 0           ; 0번째 Cylinder
            mov cl, 11           ; 0번째 섹터부터 읽기 시작한다. 
            mov dh, 0           ; Head=0
            mov dl, 0           ; Drive=0  A:드라이브
            int 13h             ; Read!

            jc read_user1       ; 에러가 나면, 다시 함. 


read_user2:
            mov ax, 0x7000      ; ES:BX = 7000:0200
            mov es, ax          ;
            mov bx, 0x200       ;

            mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로  
            mov al, 1           ; 1 섹터를 읽을 것이다.
            mov ch, 0           ; 0번째 Cylinder
            mov cl, 12          ; 0번째 섹터부터 읽기 시작한다. 
            mov dh, 0           ; Head=0
            mov dl, 0           ; Drive=0  A:드라이브
            int 13h             ; Read!

            jc read_user2       ; 에러가 나면, 다시 함. 

read_user3:
            mov ax, 0x7000      ; ES:BX = 7000:0400
            mov es, ax          ;
            mov bx, 0x400       ;

            mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로  
            mov al, 1           ; 1 섹터를 읽을 것이다.
            mov ch, 0           ; 0번째 Cylinder
            mov cl, 13          ; 0번째 섹터부터 읽기 시작한다. 
            mov dh, 0           ; Head=0
            mov dl, 0           ; Drive=0  A:드라이브
            int 13h             ; Read!

            jc read_user3       ; 에러가 나면, 다시 함. 

read_user4:
            mov ax, 0x7000      ; ES:BX = 7000:0600
            mov es, ax          ;
            mov bx, 0x600       ;

            mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로  
            mov al, 1           ; 1 섹터를 읽을 것이다.
            mov ch, 0           ; 0번째 Cylinder
            mov cl, 14          ; 0번째 섹터부터 읽기 시작한다. 
            mov dh, 0           ; Head=0
            mov dl, 0           ; Drive=0  A:드라이브
            int 13h             ; Read!

            jc read_user4       ; 에러가 나면, 다시 함. 

read_user5:
            mov ax, 0x7000      ; ES:BX = 7000:0800
            mov es, ax          ;
            mov bx, 0x800       ;

            mov ah, 2           ; 디스크에 있는 데이터를 es:bx 의 주소로  
            mov al, 1           ; 1 섹터를 읽을 것이다.
            mov ch, 0           ; 0번째 Cylinder
            mov cl, 15          ; 0번째 섹터부터 읽기 시작한다. 
            mov dh, 0           ; Head=0
            mov dl, 0           ; Drive=0  A:드라이브
            int 13h             ; Read!

            jc read_user5       ; 에러가 나면, 다시 함. 
	    
	    mov dx, 0x3F2	; 플로피 디스크 드라이브의
	    xor al, al          ; 모터를 끈다.
	    out dx, al

	    cli

            call a20_try_loop

            mov	al, 0x11		; PIC의 초기화
	    out	0x20, al		; 마스터 PIC
	    dw	0x00eb, 0x00eb		; jmp $+2, jmp $+2
	    out	0xA0, al		; 슬레이브 PIC
	    dw	0x00eb, 0x00eb

	    mov	al, 0x20		; 마스터 PIC 인터럽트 시작점
	    out	0x21, al
	    dw	0x00eb, 0x00eb
	    mov	al, 0x28		; 슬레이브 PIC 인터럽트 시작점
   	    out	0xA1, al
	    dw	0x00eb, 0x00eb

	    mov	al, 0x04		; 마스터 PIC의 IRQ2번에 
	    out	0x21, al		; 슬레이브 PIC이 연결되어 있다.
	    dw	0x00eb, 0x00eb
	    mov	al, 0x02		; 슬레이브 PIC이 마스터 PIC의
	    out	0xA1, al		; IRQ2번에 연결되어 있다.
	    dw	0x00eb, 0x00eb

	    mov	al, 0x01		; 8086 모드를 사용한다.
	    out	0x21, al
	    dw	0x00eb, 0x00eb
	    out	0xA1, al
	    dw	0x00eb, 0x00eb

	    mov	al, 0xFF		; 슬레이브 PIC의 모든 인터럽트를 
	    out	0xA1, al		; 막아 둔다.
  	    dw	0x00eb, 0x00eb
	    mov	al, 0xFB		; 마스터 PIC의 IRQ2번을 제외한
	    out	0x21, al		; 모든 인터럽트를 막아 둔다.

	    jmp 0x9000:0000   

	    msgBack db '.', 0x67
	 
	    times 510-($-$$) db 0
            dw 0AA55h

>>>>>>> prac7
