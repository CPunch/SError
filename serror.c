#include "serror.h"

#ifdef __USE_ISOC11
thread_local int __eSimple_errIndx = -1;
thread_local jmp_buf __eSimple_errStack[SIMPLE_MAXERRORS];
#else
int __eSimple_errIndx = -1;
jmp_buf __eSimple_errStack[SIMPLE_MAXERRORS];
#endif
