#include "../h/tcb.hpp"
#include "../lib/mem.h"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/print.hpp"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(TCB::Body body, void *arg) {
    return new TCB(body, arg, DEFAULT_TIME_SLICE);
}

void TCB::yield() {
    asm volatile("li a0, 0x13");

    asm volatile("ecall");
//dispatch();
}

void TCB::dispatch() {
    TCB *old = TCB::running;

    if (!old->isFinished()) Scheduler::put(old);
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();

    running->body(running->arg);
    running->setFinished(true);

//    TCB::yield();
    thread_dispatch();
}

void *operator new[](size_t size) {
//    void *ptr = mem_alloc(size);
//    printStr("tcb: new[]:");
//    printInteger((uint64) ptr);
//    printStr("\n");

    return __mem_alloc(size);
}

void operator delete[](void *ptr) noexcept {
    __mem_free(ptr);
//    printStr("tcb: delete[]:");
//    printInteger((uint64) ptr);
//    printStr("\n");
}