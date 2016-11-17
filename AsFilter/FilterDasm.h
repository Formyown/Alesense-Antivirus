#ifndef _LDASM_
#define _LDASM_



unsigned long __fastcall SizeOfCode(void *Code, unsigned char **pOpcode);

unsigned long __fastcall SizeOfProc(void *Proc);

char __fastcall IsRelativeCmd(unsigned char *pOpcode);


#endif
