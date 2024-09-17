#include "../h/allocator.hpp"

Allocator::FreeMem *Allocator::mem_head;

void Allocator::init() {
    mem_head = (FreeMem *) HEAP_START_ADDR;
    mem_head->next = nullptr;
    mem_head->prev = nullptr;
    mem_head->size = ((char *) HEAP_START_ADDR - (char *) HEAP_END_ADDR - sizeof(FreeMem));
}

void *Allocator::mem_alloc(size_t size) {
    if (size <= 0 || !Allocator::mem_head) return nullptr;

    if (size % MEM_BLOCK_SIZE != 0) {
        size = ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE) * MEM_BLOCK_SIZE;
    }

    FreeMem *first_fit = Allocator::mem_head;
    for (; first_fit; first_fit = first_fit->next) {
        if (first_fit->size >= size + sizeof(FreeMem)) break;
    }

    if (!first_fit) return nullptr;

    FreeMem *frgm;
    if (first_fit->size > size + sizeof(FreeMem)) {
        frgm = (FreeMem *) ((char *) first_fit + size + sizeof(FreeMem));
        frgm->size = first_fit->size - size - sizeof(FreeMem);
        first_fit->size = size + sizeof(FreeMem);

        frgm->next = first_fit->next;
    } else frgm = first_fit->next;

    if (first_fit->prev) {
        first_fit->prev->next = frgm;
        frgm->prev = first_fit->prev;
    } else {
        Allocator::mem_head = frgm;
        frgm->prev = nullptr;
    }

    return (char *) first_fit + sizeof(FreeMem);
}

void Allocator::tryToJoin(FreeMem *cur) {
    if (cur && cur->next && (char *) cur->next == (char *) cur + cur->size) {
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if (cur->next) {
            cur->next->prev = cur;
        }
    }
}

int Allocator::mem_free(void *ptr) {
    if ((char *) ptr - sizeof(FreeMem) < HEAP_START_ADDR
        || (char *) ptr + MEM_BLOCK_SIZE > HEAP_END_ADDR
        || !ptr)
        return -1;

    auto *newSeg = (FreeMem *) ((char *) ptr - sizeof(FreeMem));

    FreeMem *cur = mem_head;

    // newSeg = newSeg, cur = cur

    if ((char *) cur >= (char *) newSeg + newSeg->size) {
        newSeg->next = cur;
        cur->prev = newSeg;
        newSeg->prev = nullptr;
        mem_head = newSeg;
        tryToJoin(newSeg);
        return 0;
    } else {
        for (; cur; cur = cur->next) {
            if ((char *) cur + cur->size <= (char *) newSeg
                && (char *) cur->next >= (char *) newSeg + newSeg->size)
                break;
        }

        if (!cur) return -2;

        newSeg->next = cur->next;
        cur->next = newSeg;
        newSeg->prev = cur;
        newSeg->next->prev = newSeg;
        tryToJoin(cur);
        tryToJoin(newSeg);
        return 0;
    }
}



