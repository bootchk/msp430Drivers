
#pragma once

void fail(unsigned int line);


/*
 * Custom assert macros
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
