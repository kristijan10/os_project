#include "../h/allocator.hpp"

void *Allocator::mem_alloc(size_t sz) {
    static bool setH=false;
    if(!setH){
        setH=true;
        head=(FreeMem*)((uint64 *)HEAP_START_ADDR);
        head->next=nullptr;
        head->prev=nullptr;
        head->size=((uint64)((uint64 *)HEAP_END_ADDR-(uint64 *)HEAP_START_ADDR)-sizeof(Allocator))/MEM_BLOCK_SIZE;
    }
    void* ret= nullptr;
    for(FreeMem* tmp=head;tmp!=nullptr;tmp=tmp->next){
        if(tmp->size>=sz){
            ret=tmp;
            //ako je ostalo manje od velicine bloka izbacujem iz liste
            if(tmp->size-sz==0){
                if(tmp->prev!=nullptr)tmp->prev->next = tmp->next;
                else  head=tmp->next;
                if(tmp->next!=nullptr)tmp->next->prev =tmp->prev;
                tmp->size = sz;
                break;
            }
            //ako je ostalo vise od jednog bloka
            auto newNode=(FreeMem*)((uint64 *)tmp+sz*MEM_BLOCK_SIZE);
            if(tmp->prev)tmp->prev->next=newNode;
            else head=newNode;
            if(tmp->next)tmp->next->prev=newNode;
            newNode->prev=tmp->prev;
            newNode->next=tmp->next;
            newNode->size=tmp->size-sz;
            tmp->size = sz;
            break;
        }
    }
    return (uint64 *)ret+sizeof(FreeMem);
}

Allocator& Allocator::getInstance() {
    static Allocator singleton;
    return singleton;
}

void Allocator::tryToJoin(FreeMem *cur) {
    if(cur->next&& (char*)(cur->next)==((char*)cur+cur->size*MEM_BLOCK_SIZE)){
        cur->size+=cur->next->size;
        cur->next=cur->next->next;
        if(cur->next)cur->next->prev=cur;
    }

}

int Allocator::mem_free(void *adr) {
    if((uint64 *)adr>(uint64*)HEAP_END_ADDR || (uint64 *)adr<(uint64 *)HEAP_START_ADDR)return -1;
    FreeMem* prev=0;
    if(head==nullptr || (char*)adr<(char*)head) prev=0;
    else{
        for(prev=head;prev->next!=0 && (char*)adr>(char*)(prev->next);prev=prev->next);
    }
    auto seg = (FreeMem*) ((uint64*)adr - sizeof(FreeMem));
    seg->prev=prev;
    if(prev)seg->next=prev->next;
    else {
        seg->next = head;
    }
    if(seg->next)seg->next->prev=seg;
    if(prev)prev->next=seg;
    else head=seg;
    if(seg!=nullptr)tryToJoin(seg);
    if(prev!=nullptr)tryToJoin(prev);
    return 0;
}

