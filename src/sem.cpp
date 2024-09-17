#include "../h/sem.hpp"
#include "../h/print.hpp"
#include "../h/scheduler.hpp"
#include "../h/list.hpp"

void Sem::block() {
    printStr("blokiram: ");
    printInteger(TCB::running->pid);
    printStr("\n");

    numOfBlocked++;
    TCB::getRunning()->setBlocked(true);
    blocked.addLast(TCB::getRunning());
    TCB::dispatch();
}

void Sem::unblock() {
    numOfBlocked--;
    TCB *temp = blocked.removeFirst();
    temp->setBlocked(false);

    printStr("odblokiram: ");
    printInteger(TCB::running->pid);
    printStr("\n");

    Scheduler::put(temp);
    TCB::dispatch();
}

int Sem::wait() {
    if (--val < 0) block();
    if (!closed || numOfBlocked == 0) return 0;
    else {
        numOfBlocked--;
        return -1;
    }
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

    if (blocked.peekFirst() != nullptr) {
        while (blocked.peekFirst()) {
            blocked.peekFirst()->setBlocked(false);
            Scheduler::put(blocked.peekFirst());
            blocked.removeFirst();
        }
    }

    return 0;
}

//void Sem::printBlocked() {
//    blocked
//}