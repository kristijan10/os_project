#include "../h/tcb.hpp"
#include "../lib/mem.h"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/print.hpp"

extern "C" void pushRegisters();
extern "C" void popRegisters();

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(TCB::Body body, void *arg) {
    return new TCB(body, arg, DEFAULT_TIME_SLICE);
}

void TCB::yield() {
//    pushRegisters();
    asm volatile("li a0, 0x13");

    asm volatile("ecall");
//    popRegisters();
}

void TCB::dispatch() {
    TCB *old = TCB::running;

    if (!old->isFinished() && !old->isBlocked()) Scheduler::put(old);
    running = Scheduler::get();

//    if (running->body == nullptr) {
//        Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
//    } else {
//        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
//    }

//    if (old != running)
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();

    running->body(running->arg);
    running->setFinished(true);

//    TCB::yield();
    thread_dispatch();
}

//void *TCB::operator new(size_t size) {
//    void *ptr = mem_alloc(size);
//    printStr("tcb: new[]:");
//    printInteger((uint64) ptr);
//    printStr("\n");

//    return mem_alloc(size);
//}

//void TCB::operator delete(void* ptr) {
//    mem_free(ptr);
//    printStr("tcb: delete[]:");
//    printInteger((uint64) ptr);
//    printStr("\n");
//}