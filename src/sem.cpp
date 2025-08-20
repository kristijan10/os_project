#include "../h/sem.hpp"
#include "../h/print.hpp"
#include "../h/scheduler.hpp"
#include "../h/list.hpp"

void Sem::block() {
    TCB *running = TCB::getRunning();
    running->setState(TCB::BLOCKED);
    blocked.addLast(running);
    TCB::yield();
}

void Sem::unblock() {
    TCB *t = blocked.removeFirst();
    t->setState(TCB::READY);
    Scheduler::put(t);
    TCB::yield();
}

int Sem::wait() {
    if (--val < 0) block();
    if (closed) return -1;

    return 0;
}

int Sem::trywait() {
    if (val - 1 < 0) return 0;
    else return 1;
}

int Sem::timedwait(time_t timeout) {
    if (closed) return -1;
    if (val > 0) {
        val--;
        return 0;
    }

    TCB::running->setTime(timeout);
    TCB::dispatch();

    if (closed) return -1;
    if (TCB::running->getTime() > 0) return -2;
    return 0;
}

int Sem::signal() {
    if (closed) return -1;
    if (++val <= 0) unblock();
    return 0;
}

Sem *Sem::open(int init) {
    return new Sem(init);
}

int Sem::close() {
    if (closed) return -1;
    closed = true;

    while (blocked.peekFirst()) {
        TCB *t = blocked.removeFirst();
        t->setState(TCB::READY);
        Scheduler::put(t);

    }

    return 0;
}