#ifndef scheduler
#define scheduler

#include "list.hpp"

class TCB;

class Scheduler{
public:
    static TCB *get();
    static void put(TCB *tcb);

private:
    static List<TCB> readyThreadQueue;
};

#endif