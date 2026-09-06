
#pragma once

void fail(unsigned int line);


/*
 * Custom assert macros
 *
 * Just as in standard C, "assert()" is a macro.
 * Here "myAssert()" etc. are macros
 */


#ifdef NDEBUG

// assert functions generate no code
#define myAssert(_ignore)  ((void)0)
#define myRequire(_ignore) ((void)0)
#define myEnsure(_ignore)  ((void)0)

#else

// assertions generate code and are checked at runtime

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

#endif

// traditionally would be fail(__FILE__, __LINE__)
