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

    void setTime(time_t t) { this->time = t; }

    time_t getTime() const { return time; }

    int getPid() const { return pid; }

    friend class Riscv;

    friend class Sem;

    ~TCB() { delete[] stack; }

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
            finished(false),
            blocked(false),
            pid(PID++),
            time(0) {
        if (body != nullptr) Scheduler::put(this);
    }

//    TCB(TCB &parent)
//            : body(parent.body),
//              stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
//              context({parent.context.ra,
//                       parent.context.sp - (uint64) parent.stack + (uint64) stack
//                      }),
//              timeSlice(parent.timeSlice),
//              arg(parent.arg),
//              finished(parent.finished),
//              blocked(parent.blocked),
//              pid(PID++),
//              time(parent.time) {

//        printStr("body: ");
//        printInteger((uint64) body);
//        printStr("\tparent.body: ");
//        printInteger((uint64) parent.body);
//        printStr("\ncontext.sp: ");
//        printInteger((uint64) context.sp);
//        printStr("\tparent.context.sp: ");
//        printInteger((uint64) parent.context.sp);
//        printStr("\ncontext.ra: ");
//        printInteger((uint64) context.ra);
//        printStr("\tparent.context.ra: ");
//        printInteger((uint64) parent.context.ra);
//        printStr("\nstack: ");
//        printInteger((uint64) stack);
//        printStr("\tparent.stack: ");
//        printInteger((uint64) parent.stack);
//        printStr("\npid: ");
//        printInteger(pid);
//        printStr("\tparent.pid: ");
//        printInteger(parent.pid);
//
//        if (body != nullptr) {
//            printStr("\nstavio u skeduler: ");
//            printInteger(pid);
//            printStr("\n");
//            Scheduler::put(this);}
//    }

    static void dispatch();

//    static void join(thread_t *handle);

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
    time_t time;
    sem_t semJoin;
};

#endif