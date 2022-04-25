/* serror.h - Simple Error Library by CPunch

    SERROR makes errors in C syntactically similar to other languages,
    implementing try & catch blocks and is achieved using setjmp(). This helps
    fix the "return hell" you can get with a simpler error handling solution.
    The SIMPLE_ERROR(status, ...) macro is used to throw errors, and is even
    thread-safe for __USE_ISOC11.

    Define SIMPLE_MAXERRORS to change the max consecutive errors you can throw,
    and SIMPLE_QUITE to silence SIMPLE_ERRORs.

    Example:
    ```C
    // helloworld.c
    #include <stdio.h>
    #include "serror.h"

    void try() {
        puts("lo ");
        SIMPLE_ERROR(0, "Wor");
        puts("never called\n");
    }

    int main() {
        SIMPLE_TRY
            puts("Hel");
            try();
        SIMPLE_CATCH
            puts("ld!");
        SIMPLE_TRYEND

        SIMPLE_ERROR(1, "\n");

        puts("never called\n");
        return 0;
    }
    ```

    Some minor inconveniences include:
    - `return` or other control-flow statements that leave the current scope cannot be used in the SIMPLE_TRY or SIMPLE_CATCH scopes.
    - max of 32 depth (define SIMPLE_MAXERRORS), avoid using recursively.
*/

#ifndef SIMPLE_ERROR_H
#define SIMPLE_ERROR_H

#include <stdio.h>
#include <setjmp.h>

/* defines errorstack size */
#ifndef SIMPLE_MAXERRORS
# define SIMPLE_MAXERRORS 32
#endif

#ifdef __USE_ISOC11
# include <threads.h>
extern thread_local int __eSimple_errIndx;
extern thread_local jmp_buf __eSimple_errStack[SIMPLE_MAXERRORS];
#else
extern int __eSimple_errIndx;
extern jmp_buf __eSimple_errStack[SIMPLE_MAXERRORS];
#endif

/* DO NOT RETURN/GOTO/BREAK or otherwise skip SIMPLE_TRYEND */
#define SIMPLE_TRY if (setjmp(__eSimple_errStack[++__eSimple_errIndx]) == 0) {
#define SIMPLE_CATCH } else {
#define SIMPLE_TRYEND } --__eSimple_errIndx;

/* if __eSimple_errIndx is >= 0, we have a safe spot to jump too if an error is thrown */
#define SIMPLE_ISPROTECTED (__eSimple_errIndx >= 0)

/* SIMPLE_ERROR(printf args):
        if called after a SIMPLE_TRY block will jump to the previous SIMPLE_CATCH/SIMPLE_TRYEND block,
    otherwise exit(status) is called. fprintf(stderr, FORMAT, ...) is called with passed args. if SIMPLE_QUITE is defined 
    arguments are ignored.
*/
#ifdef SIMPLE_QUITE
# define SIMPLE_ERROR(status, ...) do { \
    if (SIMPLE_ISPROTECTED) \
        longjmp(__eSimple_errStack[__eSimple_errIndx], 1); \
    else \
        exit(status); \
} while(0);
#else
# define SIMPLE_ERROR(status, ...) do { \
    fprintf(stderr, __VA_ARGS__); \
    if (SIMPLE_ISPROTECTED) \
        longjmp(__eSimple_errStack[__eSimple_errIndx], 1); \
    else \
        exit(status); \
} while(0);
#endif

#endif
