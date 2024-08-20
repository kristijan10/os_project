#include "../h/allocator.hpp"

void *Allocator::mem_alloc(size_t size) {
    for (FreeMem *cur = fmem_head; cur != nullptr; cur = cur->next) {
        if (cur->size < size) continue;

        if (cur->size - size < sizeof(FreeMem)) {
            if (cur->prev) cur->prev->next = cur->next;
            else fmem_head = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            return cur;
        } else {
            auto *newfrgm = (FreeMem *) ((uint64 *) cur + size);
            if (cur->prev) cur->prev->next = newfrgm;
            else fmem_head = newfrgm;

            if (cur->next) cur->next->prev = newfrgm;

            newfrgm->prev = cur->prev;
            newfrgm->next = cur->next;
            newfrgm->size = cur->size - size;
            return cur;
        }
    }

    return nullptr;
}

//void Allocator::mem_free(void *ptr) {
//    FreeMem *cur;
//
//    if (!fmem_head || ptr < (char *) fmem_head) cur = nullptr;
//    else {
//        for (cur = fmem_head;
//             cur->next != nullptr && ptr > (char *) cur->next;
//             cur = cur->next);
//    }
//
//    if (cur && ((uint64 *) cur + cur->size == ptr)) {
//        cur->size += size;
//        if (cur->next && (char *) cur + cur->size == (char *) cur->next) {
//            cur->size += cur->next->size;
//            cur->next = cur->next->next;
//            if (cur->next) cur->next->prev = cur;
//        }
//
//        return;
//    } else if (FreeMem *nextSeg = cur ? cur->next : fmem_head && ptr + size == (char *) nextSeg) {
//        auto newSeg = (FreeMem *) ptr;
//        newSeg->size = nextSeg->size + size;
//        newSeg->prev = nextSeg->prev;
//        newSeg->next = nextSeg->next;
//        if (nextSeg->next) nextSeg->next->prev = newSeg;
//        if (nextSeg->prev) nextSeg->prev->next = newSeg;
//        else fmem_head = newSeg;
//        return;
//    } else {
//        auto newSeg = (FreeMem *) ptr;
//        newSeg->size = size;
//        newSeg->prev = cur;
//        if (cur) newSeg->next = cur->next;
//        else newSeg->next = fmem_head;
//        if (newSeg->next) newSeg->next->prev = newSeg;
//        if(cur) cur->next = newSeg;
//        else fmem_head = newSeg;
//    }
//}

void Allocator::mem_free(void *ptr) {
    FreeMem *cur = nullptr;
    if (!fmem_head || ptr < (char *) fmem_head) cur = nullptr;
    else {
        for (cur = fmem_head;
             cur->next != nullptr && ptr > (char *) cur->next;
             cur = cur->next);
    }

    auto newSeg = (FreeMem *) ptr;
//    newSeg->size = size;
    newSeg->prev = cur;

    if (cur) newSeg->next = cur->next;
    else newSeg->next = fmem_head;
    if(newSeg->next) newSeg ->next->prev = newSeg;
    if(cur) cur->next=newSeg;
    else fmem_head=newSeg;

    tryToJoin(newSeg);
    tryToJoin(cur);
}

int Allocator::tryToJoin(FreeMem *cur){
    if(!cur) return 0;

    if(cur->next && (char *)cur + cur->size == (char*) cur->next){
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if(cur->next) cur->next->prev = cur;
        return 1;
    } else return 0;
}

Allocator *Allocator::getInstance() {
    if(!allocator){
        fmem_head = (FreeMem *) ((uint64 *)HEAP_START_ADDR);
        fmem_head->next = nullptr;
        fmem_head->prev = nullptr;
        fmem_head->size = ((uint64 *) HEAP_END_ADDR - (uint64 *) HEAP_START_ADDR) / MEM_BLOCK_SIZE;
    }

    return allocator;
}