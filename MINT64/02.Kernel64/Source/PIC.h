#ifndef __PIC_H__
#define __PIC_H__

#include "Types.h"

// ��ũ��
// I/O port definition
#define PIC_MASTER_PORT1 	0x20
#define PIC_MASTER_PORT2	0x21
#define PIC_SLAVE_PORT1		0xA0
#define PIC_SLAVE_PORT2 	0xA1

// IDT Table���� Interrupt vector�� ���۵Ǵ� ��ġ, 0x20
#define PIC_IRQSTARTVECTOR  0x20

// �Լ�
void kInitializePIC(void);
void kMaskPICInterrupt(WORD wIRQBitmask);
void kSendEOIToPIC(int iIRQNumber);

#endif /*__PIC_H__*/