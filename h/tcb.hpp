#ifndef tcb_hpp
#define tcb_hpp

//#include "scheduler.hpp"
//#include "allocator.hpp"
//#include "riscv.hpp"
#include "../lib/hw.h"

class TCB {
public:
    enum State {
        CREATED, RUNNING, READY, BLOCKED, FINISHED
    };

    static bool readyToPrintA;
    static bool readyToPrintB;
    static bool readyToPrintC;

    using Body = void (*)(void *);

    bool isFinished() const { return state==FINISHED; }

    uint64 getTimeSlice() const { return timeSlice; }

    static TCB *createThread(Body body, void *arg);

    static void yield();

    static void exit();

    void start();

    static TCB *getRunning() { return running; }

    bool isBlocked() const { return state == BLOCKED; }

    void setState(State s) { state = s; }

    void setTime(time_t t) { this->time = t; }

    time_t getTime() const { return time; }

    int getPid() const { return running->pid; }

    friend class Riscv;

    friend class Sem;

    ~TCB() { delete[] stack; }

    static TCB *running;

//    bool isUser() const {return userMode;}

//    static int id_th;

private:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    TCB(Body body, void *arg, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     body != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),
            timeSlice(timeSlice),
            arg(arg),
            state(CREATED),
            pid(PID++),
            time(0)
//            userMode(Riscv::isUserMode())
            {
        if (body) start();
    }

    static void dispatch();

//    static void join(thread_t *handle);

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void threadWrapper();

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    void *arg;
    static uint64 timeSliceCounter;
    State state;
    static uint64 PID;
    int pid;
    time_t time;
//    bool userMode;
//    sem_t semJoin;
};

#endif