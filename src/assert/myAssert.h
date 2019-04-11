
#pragma once

void fail(unsigned int line);


/*
 * Custom assert macros
 *
 * Just as in standard C, "assert()" is a macro.
 * Here "myAssert()" etc. are macros
 */

/*
 * TODO
 * ifndef NDEBUG
 * define myAssert(_ignore) ((void)0)
 */


// general assertion
#define myAssert(expr) \
    if (!(expr)) \
        fail(__LINE__)


// precondition assertion
#define myRequire(expr) \
    if (!(expr)) \
        fail(__LINE__)


// postcondition assertion
#define myEnsure(expr) \
    if (!(expr)) \
        fail(__LINE__)



// traditionally would be fail(__FILE__, __LINE__)
