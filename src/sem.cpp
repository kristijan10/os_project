#include "../h/sem.hpp"

#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"

Sem *Sem::open(int init) {
    return new Sem(init);
}

int Sem::close() {
    if (closed) return -1;
    closed = true;

    while (TCB *t = blocked.removeFirst()) {
        t->setState(TCB::READY);
        Scheduler::put(t);
    }

    return 0;
}

void Sem::block() {
    TCB *running = TCB::getRunning();
    running->setState(TCB::BLOCKED);
    blocked.addLast(running);
    TCB::yield();
}

void Sem::unblock() {
    TCB *t = blocked.removeFirst();
    if (t) {
        t->setState(TCB::READY);
        Scheduler::put(t);
        TCB::yield();
    }
}

int Sem::wait() {
    if (closed) return -1;

    if (--val < 0) {
        block();
        if (closed) return -1;
    }

    return 0;
}

int Sem::signal() {
    if (closed) return -1;

    if (++val <= 0) {
        unblock();
        if (closed) return -1;
    }

    return 0;
}

int Sem::trywait() {
    if (closed) return -1;
    if (val <= 0) return -2;

    val--;
    return 0;
}

int Sem::timedwait(time_t timeout) {
    if (closed) return -1;
    if (val > 0) {
        val--;
        return 0;
    }

    TCB *running = TCB::running;
    running->setTime(timeout);
    running->setState(TCB::BLOCKED);
    TCB::yield();

    if (closed) return -1;
    if (TCB::running->getTime() > 0) return -2;
    return 0;
}