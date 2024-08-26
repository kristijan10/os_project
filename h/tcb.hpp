#ifndef tcb_hpp
#define tcb_hpp

#include "scheduler.hpp"
#include "allocator.hpp"

static int PID = 0;

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

    void setTime(int t) { this->time = t; }

    uint64 getTime() const { return time; }

    friend class Riscv;

    friend class Sem;

    ~TCB() { delete[] stack; }

    // dodatak

//    static void join(TCB *handle);

    int getPid() const { return pid; }

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
            blocked(false),
            pid(PID++),
            time(0) {
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
    int pid;
    int time;
};

#endif