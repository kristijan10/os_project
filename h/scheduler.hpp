#ifndef scheduler_hpp
#define scheduler_hpp

#include "list.hpp"

class Scheduler {
public:
    static TCB *get();
    static void put(TCB *tcb);

    static TCB *getSleep();
    static void putSleep(TCB *tcb);
    static void updateSleep();
    static TCB *findSleep(TCB *tcb);
    static void removeSleep(TCB *tcb);

//    static void print();
//    static void printSleep();

private:
    static List<TCB> readyThreadQueue;
    static List<TCB> sleepThreadQueue;
};

#endif