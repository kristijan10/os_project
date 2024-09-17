#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"
#include "../h/print.hpp"

List<TCB> Scheduler::readyThreadQueue;
List<TCB> Scheduler::sleepThreadQueue;

TCB *Scheduler::get() {
//    printStr("\nscheduler::get: ");
//    printInteger(readyThreadQueue.peekFirst()->getPid());
//    printStr("\n");
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *tcb) {
//    printStr("\nscheduler::put: ");
//    printInteger(tcb->getPid());
//    printStr("\n");
    readyThreadQueue.addLast(tcb);
}

void Scheduler::putSleep(TCB *tcb) {
//    printStr("Scheduler::putSleep\n");

    sleepThreadQueue.iter_reset();

    TCB *cur = sleepThreadQueue.iter_get_curr();
    int time = tcb->getTime(), cur_time = 0;
    bool inserted = false;
    while (cur) {
        cur_time = cur->getTime();

        if (time < cur_time) {
            tcb->setTime(time);
            cur->setTime(cur_time - time);
            sleepThreadQueue.iter_insert_before(tcb);
            inserted = true;
            break;
        }

        time -= cur_time;
        sleepThreadQueue.iter_next();
        cur = sleepThreadQueue.iter_get_curr();
    }

    if (!inserted) {
        tcb->setTime(time);
        sleepThreadQueue.addLast(tcb);
    }
}

TCB *Scheduler::getSleep() {
    if (sleepThreadQueue.peekFirst()->getTime() != 0) return nullptr;
//    printStr("Uzeo iz sleepQueue\n");
    return sleepThreadQueue.removeFirst();
}

void Scheduler::updateSleep() {
    TCB *elem = sleepThreadQueue.peekFirst();
    if (elem) {
        elem->setTime(elem->getTime() - 1);

//        printStr("pid:");
//        printInteger(elem->getPid());
//        printStr("\t");
//        printInteger(elem->getTime());
//        printStr("\n");

        while (elem && elem->getTime() <= 0) {
            Scheduler::put(elem);
            Scheduler::sleepThreadQueue.removeFirst();
//            printStr("Uklonio:");
//            printInteger((uint64)elem);
//            printStr("\n");
            elem = Scheduler::sleepThreadQueue.peekFirst();
        }
    }
}

TCB *Scheduler::findSleep(TCB *tcb) {
    return sleepThreadQueue.get(tcb);
}

void Scheduler::removeSleep(TCB *tcb) {
    sleepThreadQueue.remove(tcb);
}

//void Scheduler::printSleep(){
//    sleepThreadQueue.iter_reset();
//
//    TCB *cur = sleepThreadQueue.iter_get_curr();
//
//    printStr("sleepThreadQueue: ");
//    while (cur) {
//        printInteger(cur->getPid());
//        printStr(" ");
//
//        sleepThreadQueue.iter_next();
//        cur = sleepThreadQueue.iter_get_curr();
//    }
//    printStr("\n");
//}
//
//void Scheduler::print(){
//    readyThreadQueue.iter_reset();
//
//    TCB *cur = readyThreadQueue.iter_get_curr();
//
//    printStr("readyThreadQueue: ");
//    while (cur) {
//        printInteger(cur->getPid());
//        printStr(" ");
//
//        readyThreadQueue.iter_next();
//        cur = readyThreadQueue.iter_get_curr();
//    }
//    printStr("\n");
//}