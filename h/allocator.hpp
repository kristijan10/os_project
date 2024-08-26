#ifndef allocator_hpp
#define allocator_hpp

#include "../lib/hw.h"

class Allocator {
public:
    static Allocator &getInstance();

    void *mem_alloc(size_t sz);

    int mem_free(void *adr);

private:
    struct FreeMem {
        FreeMem *next;
        FreeMem *prev;
        size_t size;
    };

    FreeMem *fmem_head = nullptr;
    static Allocator allocator;

    static void tryToJoin(FreeMem *cur);

    void init();
};

#endif