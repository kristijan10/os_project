#include "../h/sem.hpp"
#include "../h/print.hpp"
#include "../h/scheduler.hpp"

void Sem::block() {
    numOfBlocked++;
    TCB::getRunning()->setBlocked(true);
    blocked.addLast(TCB::getRunning());
    thread_dispatch();
}

void Sem::unblock() {
    numOfBlocked--;
    TCB *temp = blocked.removeFirst();
    temp->setBlocked(false);
    Scheduler::put(temp);
}

int Sem::wait() {
    if(--val < 0) block();
    if(!closed) return 0;
    else if(numOfBlocked == 0) return 0;
    else{
        numOfBlocked--;
        return -1;
    }
}

int Sem::signal() {
    if(closed) return -1;
    if(++val <= 0) unblock();
    return 0;
}

Sem *Sem::open(int init) {
    return new Sem(init);
}

int Sem::close() {
    if(closed) return -1;
    closed = true;

    if(blocked.peekFirst() != nullptr) {
        while (blocked.peekFirst()) {
            blocked.peekFirst()->setBlocked(false);
            Scheduler::put(blocked.peekFirst());
            blocked.removeFirst();
        }
    }

    return 0;
}