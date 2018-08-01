#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t byte;

// define a struct called block, in which to store a typical 512 byte
// block of memory from a camera memory card
typedef struct
{
    byte byte1;
    byte byte2;
    byte byte3;
    byte byte4;
    byte the_rest[508];
}
block;

// define a function which returns true if the block looks like the beginning of a JPEG and returns false otherwise
bool beg_jpeg(block x)
{
    if (x.byte1 == 0xff && x.byte2 == 0xd8 && x.byte3 == 0xff &&
    (x.byte4 == 0xe0 || x.byte4 == 0xe1 || x.byte4 == 0xe2 || x.byte4 == 0xe3 || x.byte4 == 0xe4 || x.byte4 == 0xe5 ||
    x.byte4 == 0xe6 || x.byte4 == 0xe7 || x.byte4 == 0xe8 || x.byte4 == 0xe9 || x.byte4 == 0xea || x.byte4 == 0xeb ||
    x.byte4 == 0xec || x.byte4 == 0xed || x.byte4 == 0xee || x.byte4 == 0xef)
    )
        return true;
    else
        return false;
}