#include "../h/tcb.hpp"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
uint64 TCB::PID = 0;
//int TCB::id_th = -2;

TCB *TCB::createThread(TCB::Body body, void *arg) {
    return new TCB(body, arg, DEFAULT_TIME_SLICE);
}

void TCB::start() {
    this->setState(READY);
    Scheduler::put(this);
}

void TCB::yield() {
    asm volatile("li a0, 0x13");

    asm volatile("ecall");
}

void TCB::dispatch() {
    TCB *old = TCB::running;

    if (old->getTime() > 0) Scheduler::putSleep(old);
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

    running->body(running->arg);

    exit();
}

void TCB::exit() {
    running->setState(FINISHED);
    yield();
}

//void TCB::join(thread_t *handle) {
//    while (!(*handle)->isFinished()) dispatch();
//}