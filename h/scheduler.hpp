#ifndef scheduler_hpp
#define scheduler_hpp

#include "list.hpp"
//#include "tcb.hpp"

class Scheduler{
public:
    static TCB *get();
    static void put(TCB *tcb);

    static TCB *getSleep();
    static void putSleep(TCB *tcb);
    static void updateSleep();

private:
    static List<TCB> readyThreadQueue;
    static List<TCB> sleepThreadQueue;
};

#endif