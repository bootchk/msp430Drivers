
#include "myAssert.h"

#include "fatal.h"


//void fail(unsigned int line, char* file ) {

void fail(unsigned int line ) {
    Fatal::fatalAssert(line);
}
