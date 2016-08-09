#include "PIC.h"

// Initialize PIC
void kInitializePIC(void) {
	// Master PIC 초기화
	// ICW1(0x20), IC4 bit(bit 0) = 1
	kOutPortByte(PIC_MASTER_PORT1, 0x11);

	// ICW2(0x21), Vector value(0x20)
	kOutPortByte(PIC_MASTER_PORT2, PIC_IRQSTARTVECTOR);

	// ICW3(0x21)
	kOutPortByte(PIC_MASTER_PORT2, 0x04);

	// ICW4(0x21), uPM bit(bit 0) = 1
	kOutPortByte(PIC_MASTER_PORT2, 0x01);


	// Slave PIC 초기화
	// ICW1(0xA0), IC4 bit(bit 0) = 1
	kOutPortByte(PIC_SLAVE_PORT1, 0x11);

	// ICW2(0xA1), Vector value(0x20 + 8)
	kOutPortByte(PIC_SLAVE_PORT2, PIC_IRQSTARTVECTOR + 8);

	// ICW3(0xA1)
	kOutPortByte(PIC_SLAVE_PORT2, 0x02);

	// ICW4(0xA1)
	kOutPortByte(PIC_SLAVE_PORT2, 0x01);
}

void kMaskPICInterrupt(WORD wIRQBitmask) {
	// Master PIC (IMR) 설정
	// OCW1(0x21), IRQ0 ~ IRQ7
	kOutPortByte(PIC_MASTER_PORT2, (BYTE)wIRQBitmask);

	// Slave PIC (IMR) 설정
	// OCW1(0xA1), IRQ8 ~ IRQ15
	kOutPortByte(PIC_SLAVE_PORT2, (BYTE)(wIRQBitmask >> 8));
}

// EOI (End Of Interrupt)
//   master PIC controller case -> master PIC EOI
//   slave PIC controller case -> master&slave PIC EOI
void kSendEOIToPIC(int iIRQNumber) {
	// master PIC EOI 전송
	// OCW2(0x20), EOI bit(bit 5) = 1
	kOutPortByte(PIC_MASTER_PORT1, 0x20);

	// slave PIC case
	if(iIRQNumber >= 8) {
		// OCW2(0xA0), EOI bit(bit 5) = 1
		kOutPortByte(PIC_SLAVE_PORT1, 0x20);
	}
}
