#include "../h/allocator.hpp"

Allocator Allocator::allocator;

void Allocator::init() {
    if (!fmem_head) {
        fmem_head = (FreeMem *) ((uint64 *) HEAP_START_ADDR);
        fmem_head->next = nullptr;
        fmem_head->prev = nullptr;
        fmem_head->size =
                ((uint64) ((uint64 *) HEAP_END_ADDR - (uint64 *) HEAP_START_ADDR) - sizeof(Allocator)) / MEM_BLOCK_SIZE;
    }
}

void *Allocator::mem_alloc(size_t size) {
    init();

    void *tmp = nullptr;
    for (FreeMem *cur = fmem_head; cur != nullptr; cur = cur->next) {
        if (cur->size >= size) {
            tmp = cur;
            //ako je ostalo manje od velicine bloka izbacujem iz liste
            if (cur->size - size == 0) {
                if (cur->prev) cur->prev->next = cur->next;
                else fmem_head = cur->next;
                if (cur->next) cur->next->prev = cur->prev;
                cur->size = size;

                break;
            }

            //ako je ostalo vise od jednog bloka
            auto newfrgm = (FreeMem *) ((uint64 *) cur + size * MEM_BLOCK_SIZE);

            if (cur->prev) cur->prev->next = newfrgm;
            else fmem_head = newfrgm;
            if (cur->next) cur->next->prev = newfrgm;

            newfrgm->prev = cur->prev;
            newfrgm->next = cur->next;
            newfrgm->size = cur->size - size;
            cur->size = size;

            break;
        }
    }

    return (uint64 *) tmp + sizeof(FreeMem);
}

Allocator &Allocator::getInstance() {
    return allocator;
}

void Allocator::tryToJoin(FreeMem *cur) {
    if (cur && cur->next && (char *) (cur->next) == ((char *) cur + cur->size * MEM_BLOCK_SIZE)) {
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if (cur->next)cur->next->prev = cur;
    }

}

int Allocator::mem_free(void *ptr) {
    if ((uint64 *) ptr > (uint64 *) HEAP_END_ADDR || (uint64 *) ptr < (uint64 *) HEAP_START_ADDR) return -1;

    FreeMem *cur = nullptr;
    if (fmem_head == nullptr || (char *) ptr < (char *) fmem_head) cur = nullptr;
    else {
        for (cur = fmem_head;
             cur->next != nullptr && (char *) ptr > (char *) (cur->next);
             cur = cur->next);
    }

    auto newSeg = (FreeMem *) ((uint64 *) ptr - sizeof(FreeMem));
    newSeg->prev = cur;

    if (cur) newSeg->next = cur->next;
    else newSeg->next = fmem_head;

    if (newSeg->next) newSeg->next->prev = newSeg;
    if (cur) cur->next = newSeg;
    else fmem_head = newSeg;

    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0;
}

