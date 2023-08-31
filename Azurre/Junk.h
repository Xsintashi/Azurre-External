#pragma once

#if defined(_DEBUG) // asm Junk Code

#define $$$ { } ;

#else

#define $$$ __asm {  \
	__asm _emit 0xEB \
	__asm _emit 0x06 \
	__asm _emit 0xAA \
	__asm _emit 0xEE \ // You can change bytes below
	__asm _emit 0x1F \
	__asm _emit 0xAB \
	__asm _emit 0x0D \
	__asm _emit 0xCC \
	__asm _emit 0x0E \
	__asm _emit 0x13 \
	__asm _emit 0xA3 \
	__asm _emit 0xC0 \
	__asm _emit 0x87 \
} ;

#endif