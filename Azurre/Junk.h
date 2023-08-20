#pragma once

#if defined(_DEBUG) // asm Junk Code

#define $$$ { } ;

#else

#define $$$ __asm {  \
	__asm _emit 0xEB \
	__asm _emit 0x06 \
	__asm _emit 0xAA \
	__asm _emit 0xEE \
	__asm _emit 0xFF \
	__asm _emit 0xBB \
	__asm _emit 0xDD \
	__asm _emit 0xCC \
} ;

#endif