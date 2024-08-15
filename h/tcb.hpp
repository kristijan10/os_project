#ifndef tcb
#define tcb

#include "../lib/hw.h"
#include "scheduler.hpp"

class TCB {
public:
    using Body = void (*)(void *);

    bool isFinished() const { return finished; }

    void setFinished(bool val) { finished = val; }

    uint64 getTimeSlice() const { return timeSlice; }

    static TCB *createThread(Body body, void *arg);

    static void yield();

    static TCB *getRunning() { return running; }

    static void setRunning(TCB *runn) { running = runn; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool val) { blocked = val; }

    friend class Riscv;

    friend class Sem;

//    void *operator new(size_t size){return mem_alloc(size);}

//    void operator delete(void *ptr){ mem_free(ptr);}

    ~TCB() { delete[] stack; }

private:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    explicit TCB(Body body, void *arg, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     body != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),
            timeSlice(timeSlice),
            arg(arg),
            finished(false),
            blocked(false) {
        if (body != nullptr) Scheduler::put(this);
    }

    static void dispatch();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void threadWrapper();

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    void *arg;
    bool finished;
    static uint64 timeSliceCounter;
    static TCB *running;
    bool blocked;
};

#endif