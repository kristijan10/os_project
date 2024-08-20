#ifndef mem_allocator
#define mem_allocator

#include "../lib/hw.h"

class Allocator{
public:
    Allocator *getInstance();

    void *mem_alloc(size_t size);
    void mem_free(void *ptr);
private:
    struct FreeMem{
        FreeMem *next, *prev;
        size_t size;
    };

    static Allocator *allocator;
    Allocator(){}
    FreeMem *fmem_head;

    static int tryToJoin(FreeMem *cur);
};

#endif