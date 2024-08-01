#include "../h/tcb.hpp"
#include "../lib/mem.h"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body) {
    return new TCB(body, TIME_SLICE);
}

void TCB::yield() {
    Riscv::pushRegisters();

    TCB::dispatch();

    Riscv::popRegisters();
}

void TCB::dispatch() {
    TCB *old = TCB::running;

    if (!old->isFinished()) Scheduler::put(old);
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void *operator new[](uint64 size) { return __mem_alloc(size); }

void operator delete[](void *ptr) noexcept { __mem_free(ptr); }