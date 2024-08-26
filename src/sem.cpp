#include "../h/sem.hpp"
#include "../h/print.hpp"
#include "../h/scheduler.hpp"
#include "../test/printing.hpp"

void Sem::block() {
    numOfBlocked++;
    TCB::getRunning()->setBlocked(true);
    blocked.addLast(TCB::getRunning());
    TCB::dispatch();
}

void Sem::unblock() {
    numOfBlocked--;
    TCB *temp = blocked.removeFirst();
    temp->setBlocked(false);
//    if(time > 0) izvaditi iz sleepQueue, time = 0;
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
    if (val < 0) return 0;
    else return 1;
}

int Sem::signal() {
    if (closed) return -1;
    if (++val <= 0) unblock();
    return 0;
}

int Sem::timedwait(time_t timeout) {
    // ubaci u blokiran i uspavan niz, uspavljuje ga na timeout vremena
    // ako se dogodi da je odblokiran (da se pojavio signal pre isteka
    // timeout vremena) treba ga izbaciti i iz sleepQueue
    // i umetnuti u readyQueue
    printString("Usao u sem->timedwait\npid:");
    TCB *old = TCB::getRunning();
    printInteger(old->getPid());
    printStr("\n");


    old->setTime(timeout);
    printStr("Idem da cekam\t");
    printInteger(timeout);
    printStr("\n");
    if (--val < 0) block();

    int ret = 0;
    if (Scheduler::findSleep(old)) {
        // izbaciti ga iz sleep
        Scheduler::removeSleep(old);
        ret = -1;
    } else if (blocked.get(old)) {
        // izbaciti iz blocked
        blocked.remove(old);
        ret = -2;
    }

//    Scheduler::put(old);
    TCB::dispatch();
    return ret;
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