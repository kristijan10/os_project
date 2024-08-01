#include "../h/syscall_cpp.hpp"
#include "../lib/mem.h"

void *operator new(size_t size) { return __mem_alloc(size); }

void operator delete(void *ptr) noexcept { __mem_free(ptr); }