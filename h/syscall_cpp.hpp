#ifndef syscall_cpp
#define syscall_cpp

#include "../lib/hw.h"
#include "../h/syscall_c.hpp"

void *operator new(size_t size);
void operator delete (void *ptr) noexcept;

class Thread{
public:
    Thread(void (*body)(void *), void *arg);
    virtual ~Thread();

    int start();

    static void dispatch();
    static int sleep(time_t time);

protected:
    Thread();
    virtual void run();

private:
    thread_t myHandle;
    void (*body)(void *);
    void *arg;
};

class Semaphore{
public:
    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();
    int timedWait(time_t time);
    int tryWait();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread{
public:
    void terminate();

protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation(){}

private:
    time_t period;
};

class Console{
public:
    static char getc();
    static void putc(char c);
};

#endif