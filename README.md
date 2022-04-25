# serror.h

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
