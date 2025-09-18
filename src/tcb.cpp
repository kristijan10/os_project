#include "../h/tcb.hpp"

#include "../h/riscv.hpp"
<<<<<<< HEAD
#include "../h/syscall_c.h"
#include "../h/print.hpp"
=======
#include "../h/scheduler.hpp"
>>>>>>> final3

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
uint64 TCB::PID = 0;
bool TCB::readyToPrintA = false;
bool TCB::readyToPrintB = false;
bool TCB::readyToPrintC = false;
//int TCB::id_th = -2;

TCB *TCB::createThread(TCB::Body body, void *arg) {
    return new TCB(body, arg, DEFAULT_TIME_SLICE);
}

<<<<<<< HEAD
TCB *TCB::createThread(Body body, void *arg) {
    printStr("TCB::createThread\n");
    return new TCB(body, arg, DEFAULT_TIME_SLICE);
=======
void TCB::start() {
    this->setState(READY);
    Scheduler::put(this);
>>>>>>> final3
}

void TCB::yield() {
    asm volatile("li a0, 0x13");
<<<<<<< HEAD
=======

>>>>>>> final3
    asm volatile("ecall");
}

void TCB::dispatch() {
    TCB *old = TCB::running;

    if (old->getTime() > 0) {
        old->setState(BLOCKED);
        Scheduler::putSleep(old);
    }
    else if (!old->isFinished() && !old->isBlocked()) {
        old->setState(READY);
        Scheduler::put(old);
    }

    running = Scheduler::get();
    running->setState(RUNNING);

    if (running != old)
        contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();

<<<<<<< HEAD
    running->body(running->arg);
    running->setFinished(true);

    TCB::yield();
}

void *operator new[](uint64 size) {
    printStr("new[]\n");
    return __mem_alloc(size); }

void operator delete[](void *ptr) noexcept {
    printStr("delete[]\n");
    __mem_free(ptr); }
=======
    if (running->body) {
        running->body(running->arg);
        exit();
    }
}

void TCB::exit() {
    running->setState(FINISHED);
    yield();
}

//void TCB::join(thread_t *handle) {
//    while (!(*handle)->isFinished()) dispatch();
//}
>>>>>>> final3
