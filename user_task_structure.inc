;****************************************
;**********User1 Task_Sturcture**********
;****************************************
times 63 dd 0
User1Stack:
User1regs:
  dd 0, 0, 0, 0, 0, 0, 0, 0

  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0

  dd 0x80000000
  dw UserCodeSelector, 0
  dd 0x200
  dd 0x80000FFF
  dw UserDataSelector, 0

;****************************************
;**********User1 Task_Sturcture**********
;****************************************
times 63 dd 0
User2Stack:
User2regs:
  dd 0, 0, 0, 0, 0, 0, 0, 0

  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0

  dd 0x80001000
  dw UserCodeSelector, 0
  dd 0x200
  dd 0x80001FFF
  dw UserDataSelector, 0

;****************************************
;**********User3 Task_Sturcture**********
;****************************************
times 63 dd 0
User3Stack:
User3regs:
  dd 0, 0, 0, 0, 0, 0, 0, 0

  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0

  dd 0x80002000
  dw UserCodeSelector, 0
  dd 0x200
  dd 0x80002FFF
  dw UserDataSelector, 0

;****************************************
;**********User4 Task_Sturcture**********
;****************************************
times 63 dd 0
User4Stack:
User4regs:
  dd 0, 0, 0, 0, 0, 0, 0, 0

  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0

  dd 0x80003000
  dw UserCodeSelector, 0
  dd 0x200
  dd 0x80003FFF
  dw UserDataSelector, 0

;****************************************
;**********User5 Task_Sturcture**********
;****************************************
times 63 dd 0
User5Stack:
User5regs:
  dd 0, 0, 0, 0, 0, 0, 0, 0

  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0
  dw UserDataSelector, 0

  dd 0x80004000
  dw UserCodeSelector, 0
  dd 0x200
  dd 0x80004FFF
  dw UserDataSelector, 0
