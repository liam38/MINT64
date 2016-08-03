#include "Types.h"
#include "AssemblyUtility.h"
#include "Keyboard.h"

//////////////////////////////
//Ű���� ��Ʈ�ѷ��� Ű���� ��� ���õ� �Լ�
//////////////////////////////

//  ��� ����(port 0x60)�� ���ŵ� �����Ͱ� �ִ��� ���θ� ��ȯ
BOOL kIsOutputBufferFull(void) {
	//���� reg(port 0x64)���� ���� ���� ��¹��� ���� ��Ʈ(bit 0)��
	//1�� set�̸�, ��� ���ۿ� Ű���尡 ������ �����Ͱ� ������
	if(kInPortByte(0x64) & 0x01)
		return TRUE;
	return FALSE;
}

//  �Է� ����(port 0x60)�� ���μ����� �� �����Ͱ� �����ִ��� ���θ� ��ȯ
BOOL kIsInputBufferFull(void) {
	//���� reg(port 0x64)���� ���� ���� �Է¹��� ���� ��Ʈ(bit 1)��
	//1�� set�̸�, ���� Ű���尡 �����͸� �������� �ʾ���
	if(kInPortByte(0x64) & 0x02)
		return TRUE;
	return FALSE;
}

//  Ű���� Ȱ��ȭ
BOOL kActivateKeyboard(void) {
	int i, j;

	//��Ʈ�� reg(port 0x64)�� Ű���� Ȱ��ȭ Ŀ�ǵ�(0xAE) ����
	kOutPortByte(0x64, 0xAE);

	//�Է¹���(port 0x60)�� �������� ��� ��, Ű���忡 ����
	for(i=0; i<0xFFFF; i++)
		if(kIsInputBufferFull() == FALSE)
			break;

	//�Է¹���(port 0x60)�� Ű���� Ȱ��ȭ(0xF4)�� ������ Ű����� ����
	kOutPortByte(0x60, 0xF4);

	//ACK ���
	//ACK�� ���� ���� Ű���� ��� ����(port 0x60)�� Ű �����Ͱ� ����Ǿ� ���� �� �����Ƿ�
	//Ű���忡�� ���޵� �����͸� �ִ� 100������ �����Ͽ� ACKȮ��
	for(j=0; j<100; j++)
		for(i=0; i<0xFFFF; i++)
			if(kIsOutputBufferFull() == TRUE)
				break;

	//��¹���(port 0x60)���� ���� �����Ͱ� ACK(0xFA)�� ����
	if(kInPortByte(0x60) == 0xFA)
		return TRUE;

	return FALSE;
}

//  ��¹��� (port 0x60)���� Ű�� ����
BYTE kGetKeyboardScanCode(void) {
	//��¹���(port 0x60)�� �����Ͱ� ���������� ���
	while(kIsOutputBufferFull() == FALSE);

	return kInPortByte(0x60);
}

//  Ű���� LED�� ON/OFF ����
BOOL kChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn) {
	int i, j;

	//Ű���忡 LED Ŀ�ǵ� ������, ó���� ���� ���
	for(i=0; i<0xFFFF; i++)
		if(kIsInputBufferFull() == FALSE)
			break;

	//��¹���(port 0x60)���� LED ���� ���� Ŀ�ǵ�(0xED) ����
	kOutPortByte(0x60, 0xED);
	for(i=0; i<0xFFFF; i++)
		if(kIsInputBufferFull()==FALSE)
			break;

	//Ű���尡 LED Ŀ�ǵ带 ���������Ƿ� ACK ���
	for(j=0; j<100; j++) {
		for(i=0; i<0xFFFF; i++)
			if(kIsOutputBufferFull() == TRUE)
				break;

		//��¹���(port 0x60)���� ���� �����Ͱ� ACK�� ����
		if(kInPortByte(0x60) == 0xFA)
			break;
	}

	if(j >= 100)
		return FALSE;

	//LED ���� ���� Ű����� ����, �����Ͱ� ó���ɋ����� ���
	kOutPortByte(0x60, (bCapsLockOn <<2) | (bNumLockOn << 1) | bScrollLockOn);
	for(i=0; i<0xFFFF; i++)
		if(kIsInputBufferFull() == FALSE)
			break;

	//Ű���尡 LED �����͸� ���������Ƿ� ACK�� �ö����� ���
	for(j=0; j<100; j++) {
		for(i=0; i<0xFFFF; i++)
			if(kIsOutputBufferFull() == TRUE)
				break;

		//��¹���(port 0x60)���� ���� �����Ͱ� ACK(0xFA)�� ����
		if(kInPortByte(0x60) == 0xFA)
			break;
	}

	if(j >= 100)
		return FALSE;

	return TRUE;
}

//  A20 ����Ʈ Ȱ��ȭ
void kEnableA20Gate(void) {
	BYTE bOutputPortData;
	int index;

	//��Ʈ�� reg(port 0x64)�� Ű���� ��Ʈ�ѷ��� �����Ʈ ���� �д� Ŀ�ǵ�(0xD0) ����
	kOutPortByte(0x64, 0xD0);

	//��� ��Ʈ�� �����͸� ��ٷȴٰ� ����
	for(index=0; index < 0xFFFF; index++)
		if(kIsOutputBufferFull() == TRUE)
			break;

	//�����Ʈ (port 0x60)�� ���ŵ� Ű���� ��Ʈ�ѷ��� ��� ��Ʈ �� ����
	bOutputPortData = kInPortByte(0x60);

	//A20 ����Ʈ ��Ʈ ����
	bOutputPortData |= 0x01;

	//�Է¹��� (port 0x60)�� �����Ͱ� ��� ������ ��� ��Ʈ�� ���� ���� Ŀ�ǵ�� ��� ��Ʈ ������ ����
	for(index=0; index < 0xFFFF; index++)
		if(kIsInputBufferFull() == FALSE)
			break;

	//Ŀ�ǵ� reg(0x64)�� �����Ʈ ���� Ŀ�ǵ�(0xD1) ����
	kOutPortByte(0x64, 0xD1);

	//�Է¹���(0x60)�� A20 ����Ʈ ��Ʈ�� 1�� ������ ���� ����
	kOutPortByte(0x60, bOutputPortData);
}

//���μ��� ����
void kReboot(void) {
	int index;

	//�Է¹���(0x60)�� �����Ͱ� ��� ������ �����Ʈ�� ���� ���� Ŀ�ǵ�� �����Ʈ ������ ����
	for(index=0; index < 0xFFFF; index++)
		//�Է¹���(0x60)�� ������� Ŀ�ǵ� ���� ����
		if(kIsInputBufferFull() == FALSE)
			break;

	//Ŀ�ǵ� reg(0x64)�� �����Ʈ ���� Ŀ�ǵ�(0xD1)�� ����
	kOutPortByte(0x64, 0xD1);

	//�Է¹���(0x60)�� 0�� �����Ͽ� ���μ��� ����
	kOutPortByte(0x60, 0x00);

	while(1);
}
///////////////////////////////////
//��ĵ�ڵ带 ASCII�� ��ȯ�ϴ� ��ɿ� ���õ� �Լ���
///////////////////////////////////

//Ű���� ���¸� �����ϴ� Ű���� �Ŵ���
static KEYBOARDMANAGER gs_stKeyboardManager = {0};

//��ĵ�ڵ带 ASCII�ڵ�� ��ȯ�ϴ� ���̺�
static KEYMAPPINGENTRY gs_vstKeyMappingTable[KEY_MAPPINGTABLEMAXCOUNT] = {
		/* 0 */ {	KEY_NONE		,	KEY_NONE		},
		/* 1 */ {	KEY_ESC			,	KEY_ESC			},
		/* 2 */ {	'1'				,	'!'				},
		/* 3 */ {	'2'				,	'@'				},
		/* 4 */ {	'3'				,	'#'				},
		/* 5 */ {	'4'				,	'$'				},
		/* 6 */ {	'5'				,	'%'				},
		/* 7 */ {	'6'				,	'^'				},
		/* 8 */ {	'7'				,	'&'				},
		/* 9 */ {	'8'				,	'*'				},
		/* 10 */ {	'9'				,	'('				},
		/* 11 */ {	'0'				,	')'				},
		/* 12 */ {	'-'				,	'_'				},
		/* 13 */ {	'='				,	'+'				},
		/* 14 */ {	KEY_BACKSPACE	,	KEY_BACKSPACE	},
		/* 15 */ {	KEY_TAB			,	KEY_TAB			},
		/* 16 */ {	'q'				,	'Q'				},
		/* 17 */ {	'w'				,	'W'				},
		/* 18 */ {	'e'				,	'E'				},
		/* 19 */ {	'r'				,	'R'				},
		/* 20 */ {	't'				,	'T'				},
		/* 21 */ {	'y'				,	'Y'				},
		/* 22 */ {	'u'				,	'U'				},
		/* 23 */ {	'i'				,	'I'				},
		/* 24 */ {	'o'				,	'O'				},
		/* 25 */ {	'p'				,	'P'				},
		/* 26 */ {	'['				,	'{'				},
		/* 27 */ {	']'				,	'}'				},
		/* 28 */ {	'\n'			,	'\n'			},
		/* 29 */ {	KEY_CTRL		,	KEY_CTRL		},
		/* 30 */ {	'a'				,	'A'				},
		/* 31 */ {	's'				,	'S'				},
		/* 32 */ {	'd'				,	'D'				},
		/* 33 */ {	'f'				,	'F'				},
		/* 34 */ {	'g'				,	'G'				},
		/* 35 */ {	'h'				,	'H'				},
		/* 36 */ {	'j'				,	'J'				},
		/* 37 */ {	'k'				,	'K'				},
		/* 38 */ {	'l'				,	'L'				},
		/* 39 */ {	';'				,	':'				},
		/* 40 */ {	'\''			,	'\"'			},
		/* 41 */ {	'`'				,	'~'				},
		/* 42 */ {	KEY_LSHIFT		,	KEY_LSHIFT		},
		/* 43 */ {	'\\'			,	'|'				},
		/* 44 */ {	'z'				,	'Z'				},
		/* 45 */ {	'x'				,	'X'				},
		/* 46 */ {	'c'				,	'C'				},
		/* 47 */ {	'v'				,	'V'				},
		/* 48 */ {	'b'				,	'B'				},
		/* 49 */ {	'n'				,	'N'				},
		/* 50 */ {	'm'				,	'M'				},
		/* 51 */ {	','				,	'<'				},
		/* 52 */ {	'.'				,	'>'				},
		/* 53 */ {	'/'				,	'?'				},
		/* 54 */ {	KEY_RSHIFT		,	KEY_RSHIFT		},
		/* 55 */ {	'*'				,	'*'				},
		/* 56 */ {	KEY_LALT		,	KEY_LALT		},
		/* 57 */ {	' '				,	' '				},
		/* 58 */ {	KEY_CAPSLOCK	,	KEY_CAPSLOCK	},
		/* 59 */ {	KEY_F1			,	KEY_F1			},
		/* 60 */ {	KEY_F2			,	KEY_F2			},
		/* 61 */ {	KEY_F3			,	KEY_F3			},
		/* 62 */ {	KEY_F4			,	KEY_F4			},
		/* 63 */ {	KEY_F5			,	KEY_F5			},
		/* 64 */ {	KEY_F6			,	KEY_F6			},
		/* 65 */ {	KEY_F7			,	KEY_F7			},
		/* 66 */ {	KEY_F8			,	KEY_F8			},
		/* 67 */ {	KEY_F9			,	KEY_F9			},
		/* 68 */ {	KEY_F10			,	KEY_F10			},
		/* 69 */ {	KEY_NUMLOCK		,	KEY_NUMLOCK		},
		/* 70 */ {	KEY_SCROLLLOCK	,	KEY_SCROLLLOCK	},

		//���⼭ ���ʹ� Desktop keyboard�� key pad..
		/* 71 */ {	KEY_HOME		,	'7'				},
		/* 72 */ {	KEY_UP			,	'8'				},
		/* 73 */ {	KEY_PAGEUP		,	'9'				},
		/* 74 */ {	'-'				,	'-'				},
		/* 75 */ {	KEY_LEFT		,	'4'				},
		/* 76 */ {	KEY_CENTER		,	'5'				},
		/* 77 */ {	KEY_RIGHT		,	'6'				},
		/* 78 */ {	'+'				,	'+'				},
		/* 79 */ {	KEY_END			,	'1'				},
		/* 80 */ {	KEY_DOWN		,	'2'				},
		/* 81 */ {	KEY_PAGEDOWN	,	'3'				},
		/* 82 */ {	KEY_INS			,	'0'				},
		/* 83 */ {	KEY_DEL			,	'.'				},
		/* 84 */ {	KEY_NONE		,	KEY_NONE		},
		/* 85 */ {	KEY_NONE		,	KEY_NONE		},
		/* 86 */ {	KEY_NONE		,	KEY_NONE		},
		/* 87 */ {	KEY_F11			,	KEY_F11			},
		/* 88 */ {	KEY_F12			,	KEY_F12			}
};

//  ��ĵ �ڵ尡 ���ĺ� �������� ���θ� ��ȯ
BOOL kIsAlphabetScanCode(BYTE bScanCode) {
	//��ȯ ���̺��� ���� ���� �о� ���ĺ� �������� Ȯ��
	if(('a' <= gs_vstKeyMappingTable[bScanCode].bNormalCode) &&
			(gs_vstKeyMappingTable[bScanCode].bNormalCode <= 'z'))
		return TRUE;

	return FALSE;
}

//  ���� �Ǵ� ��ȣ �������� ���θ� ��ȯ
BOOL kIsNumberOrSymbolScanCode(BYTE bScanCode) {
	//�����е峪 Ȯ�� Ű ������ ������ ����(��ĵ�ڵ� 2~53)���� �����ڰ� �ƴϸ�
	//���ڳ� ��ȣ��
	if((2 <= bScanCode) && (bScanCode <= 53) &&
			(kIsAlphabetScanCode(bScanCode) == FALSE))
		return TRUE;

	return FALSE;
}

//  ���� �е� �������� ���θ� ��ȯ
BOOL kIsNumberPadScanCode(BYTE bScanCode) {
	//���� �е�� ��ĵ�ڵ��� 71~83�� ����
	if((71 <= bScanCode) && (bScanCode <= 83))
		return TRUE;

	return FALSE;
}

//  ���յ� Ű ���� ����ؾ� �ϴ��� ���θ� ��ȯ
BOOL kIsUseCombinedCode(BOOL bScanCode) {
	BYTE bDownScanCode;
	BOOL bUseCombinedKey;

	bDownScanCode = bScanCode & 0x7F;

	//���ĺ� Ű��� Shift Ű�� Caps Lock�� ������ ����
	if(kIsAlphabetScanCode(bDownScanCode) == TRUE) {
		//���� Shift Ű�� Caps Lock Ű �߿� �ϳ��� ������ ������ ���յ� Ű�� ������
		if(gs_stKeyboardManager.bShiftDown ^ gs_stKeyboardManager.bCapsLockDown)
			bUseCombinedKey = TRUE;
		else
			bUseCombinedKey = FALSE;
	}

	//���ڿ� ��ȣ Ű��� Shift Ű�� ������ ����
	else if(kIsNumberOrSymbolScanCode(bDownScanCode) == TRUE) {
		//Shift Ű�� ������ ������ ���յ� Ű�� ������
		if(gs_stKeyboardManager.bShiftDown == TRUE)
			bUseCombinedKey = TRUE;
		else
			bUseCombinedKey = FALSE;
	}

	//���� �е� Ű��� Num LockŰ�� ������ ����
	//0xE0�� �����ϸ� Ȯ�� Ű �ڵ�� ���� �е��� �ڵ尡 ��ġ�Ƿ�,
	//Ȯ�� Ű �ڵ尡 ���ŵ��� �ʾ��� ���� ó�� ���յ� �ڵ� ���
	else if((kIsNumberPadScanCode(bDownScanCode) == TRUE) &&
			(gs_stKeyboardManager.bExtendedCodeIn == FALSE)) {
		//Num Lock Ű�� ������ ������, ���յ� Ű�� �ǵ��� ��
		if(gs_stKeyboardManager.bNumLockDown == TRUE)
			bUseCombinedKey = TRUE;
		else
			bUseCombinedKey = FALSE;
	}

	return bUseCombinedKey;
}

//  ���� Ű�� ���¸� �����ϰ� LED ���� ����ȭ
void UpdateCombinationKeyStatusAndLED(BYTE bScanCode) {
	BOOL bDown;
	BYTE bDownScanCode;
	BOOL bLEDStatusChanged = FALSE;

	//����, ������ ����ó��, �ֻ��� ��Ʈ(bit 7)�� 1�̸� Ű�� ���������� �ǹ�
	// 0�̸� ���������� �ǹ���
	if(bScanCode & 0x80) {
		bDown = FALSE;
		bDownScanCode = bScanCode & 0x7F;
	}

	else {
		bDown = TRUE;
		bDownScanCode = bScanCode;
	}

	//���� Ű �˻�
	//Shift Ű�� ��ĵ�ڵ� (42 or 54)�̸� Shift Ű�� ���� ����
	if((bDownScanCode == 42) || (bDownScanCode == 54))
		gs_stKeyboardManager.bShiftDown = bDown;
	//Caps Lock Ű�� ��ĵ�ڵ� (58)�̸� Caps Lock ���� �����ϰ� LED ���� ����
	else if((bDownScanCode == 58) && (bDown == TRUE)) {
		gs_stKeyboardManager.bCapsLockDown ^= TRUE;
		bLEDStatusChanged = TRUE;
	}
	//Num LockŰ�� ��ĵ�ڵ�(69)�̸� Num Lock ...
	else if((bDownScanCode == 69) && (bDown == TRUE)) {
		gs_stKeyboardManager.bNumLockDown ^= TRUE;
		bLEDStatusChanged = TRUE;
	}
	//Scroll Lock Ű�� ��ĵ�ڵ�(70)�̸� ...
	else if((bDownScanCode == 70) && (bDown == TRUE)) {
		gs_stKeyboardManager.bScrollLockDown ^= TRUE;
		bLEDStatusChanged = TRUE;
	}

	//LED ���°� �������� Ű����� Ŀ�ǵ带 �����Ͽ� LED ����
	if(bLEDStatusChanged == TRUE)
		kChangeKeyboardLED(gs_stKeyboardManager.bCapsLockDown,
				gs_stKeyboardManager.bNumLockDown, gs_stKeyboardManager.bScrollLockDown);
}

//  ��ĵ�ڵ带 ASCII �ڵ�� ��ȯ
BOOL kConvertScanCodeToASCIICode(BYTE bScanCode, BYTE* pbASCIICode, BOOL* pbFlags) {
	BOOL bUseCombinedKey;

	//������ Pause Ű�� ���ŵǾ��ٸ�, Pause�� ���� ��ĵ�ڵ� ����
	if(gs_stKeyboardManager.iSkipCountForPause > 0) {
		gs_stKeyboardManager.iSkipCountForPause--;
		return FALSE;
	}

	//Pause Ű�� Ư���� ó��
	if(bScanCode == 0xE1) {
		*pbASCIICode = KEY_PAUSE;
		*pbFlags = KEY_FLAGS_DOWN;
		gs_stKeyboardManager.iSkipCountForPause = KEY_SKIPCOUNTFORPAUSE;
		return TRUE;
	}

	//Ȯ�� Ű �ڵ尡 ������ ��, ���� Ű ���� ������ �����Ƿ� �÷��� ������ �ϰ� ����
	else if(bScanCode == 0xE0) {
		gs_stKeyboardManager.bExtendedCodeIn = TRUE;
		return FALSE;
	}

	//���յ� Ű�� ��ȯ�ؾ� �ϴ°�?
	bUseCombinedKey = kIsUseCombinedCode(bScanCode);

	//Ű �� ����
	if(bUseCombinedKey == TRUE)
		*pbASCIICode = gs_vstKeyMappingTable[ bScanCode & 0x7F ].bCombinedCode;
	else
		*pbASCIICode = gs_vstKeyMappingTable[ bScanCode & 0x7F ].bNormalCode;

	//Ȯ�� Ű ���� ����
	if(gs_stKeyboardManager.bExtendedCodeIn == TRUE) {
		*pbFlags = KEY_FLAGS_EXTENDEDKEY;
		gs_stKeyboardManager.bExtendedCodeIn = FALSE;
	}
	else
		*pbFlags = 0;

	//�������̳� ������ ���� ����
	if((bScanCode & 0x80) == 0)
		*pbFlags |= KEY_FLAGS_DOWN;

	//���� Ű �����̳� ������ ���¸� ����
	UpdateCombinationKeyStatusAndLED(bScanCode);
	return TRUE;
}