#include "../h/sem.hpp"

void Sem::block() {
//    TCB *thread = TCB::getRunning();
    blocked.addLast(TCB::getRunning());
//    blocked.addLast(thread);
//    TCB::getRunning()->setBlocked(true);

    thread_dispatch();
//    TCB::setRunning(Scheduler::get());
//    if(TCB::getRunning() != thread) TCB::contextSwitch(&thread->context, &TCB::getRunning()->context);
}

void Sem::unblock() {
    TCB *thread = blocked.removeFirst();

    if(thread) Scheduler::put(thread);
}

int Sem::wait() {
    TCB *old = TCB::getRunning();

    if (--val < 0) {
      blocked.addLast(TCB::getRunning());
    } else Scheduler::put(old);

    TCB::setRunning(Scheduler::get());

    TCB::contextSwitch(&old->context, &TCB::getRunning()->context);
    return 0;
}

int Sem::signal() {
    if (++val <= 0) unblock();

    return 0;
}

Sem *Sem::open(int init) {
    return new Sem(init);
}

int Sem::close() {
    while(blocked.peekFirst()){
        Scheduler::put(blocked.removeFirst());
    }

    return 0;
}