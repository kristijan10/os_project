#ifndef allocator_hpp
#define allocator_hpp

#include "../lib/hw.h"

class Allocator {
public:
    static void *mem_alloc(size_t sz);

    static int mem_free(void *adr);

    static void init();

private:
    struct FreeMem {
        FreeMem *next, *prev;
        size_t size;
    };

    static FreeMem *mem_head;

    static void tryToJoin(FreeMem *cur);
};

#endif