#ifndef syscall_cpp_hpp
#define syscall_cpp_hpp

#include "syscall_c.hpp"
//#include "allocator.hpp"

// ============= MEMORIJA =============
void *operator new (size_t size);
void operator delete (void *ptr) noexcept;

// ============= SEMAFOR =============
class Semaphore{
public:
    explicit Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();
    int timedWait(time_t time);
    int tryWait();

private:
    sem_t myHandle;
};

// ============= KONZOLA =============
class Console{
public:
    static char getc();
    static void putc(char c);
};

// ============= NITI =============
class Thread{
public:
    Thread(void (*body)(void *), void *arg);
    virtual ~Thread();

    int start();
//    static void setMaxThreadNum(int num){
//        sem = new Semaphore(num);
//        maxNumOfThreads = num;
//        maxThread = true;
//    }
//    int getThId(){return id;}

    static void dispatch();
    static int sleep(time_t time);
//    void join();

protected:
    Thread();
    virtual void run(){}

private:
    thread_t myHandle;
    static Semaphore *sem;
    void (*body)(void *);
    void *arg;
//    int id;
//    static bool maxThread;
//    static int maxNumOfThreads;

    static void wrapper(void *thread);
};

// ============= PERIODICNE NITI =============
class PeriodicThread : public Thread{
public:
    void terminate();

protected:
    explicit PeriodicThread(time_t period);
    virtual void periodicActivation(){}

private:
    time_t period;
    void run();
};

#endif