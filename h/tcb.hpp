#ifndef tcb
#define tcb

#include "../lib/hw.h"
#include "scheduler.hpp"

void *operator new[](uint64 size);
void operator delete[](void *ptr) noexcept;

class TCB {
public:
    using Body = void (*)();

    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool val) { finished = val; }

    uint64 getTimeSlice() const{return timeSlice;}

    static TCB *createThread(Body body);

    static void yield();

    static void threadWrapper();

    friend class Riscv;

    static TCB *running;

private:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    explicit TCB(Body body, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     body != nullptr ? (uint64) &stack[STACK_SIZE] : 0}),
            timeSlice(timeSlice),
            finished(false) {
        if (body != nullptr) Scheduler::put(this);
    }

    static void dispatch();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif