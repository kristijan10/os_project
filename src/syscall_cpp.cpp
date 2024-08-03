#include "../h/syscall_cpp.hpp"
#include "../lib/mem.h"

// ============= MEMORIJA =============
void *operator new(size_t size) { return __mem_alloc(size); }

void operator delete(void *ptr) noexcept { __mem_free(ptr); }

// ============= NITI =============
Thread::Thread(void (*body)(void *), void *arg) :
        myHandle(nullptr),
        body(body),
        arg(arg) {
    thread_create(&myHandle, body, arg);
}

Thread::~Thread() {}

int Thread::start() { return 0; }

void Thread::dispatch() { thread_dispatch(); }

int Thread::sleep(time_t time) { return time_sleep(time); }

Thread::Thread() :
        myHandle(nullptr),
        body(nullptr),
        arg(nullptr) {}

// ============= SEMAFOR =============
Semaphore::Semaphore(unsigned int init) : myHandle(nullptr) { sem_open(&myHandle, init); }

Semaphore::~Semaphore() { sem_close(myHandle); }

int Semaphore::wait() { return sem_wait(myHandle); }

int Semaphore::signal() { return sem_signal(myHandle); }

int Semaphore::timedWait(time_t time) { return sem_timedwait(myHandle, time); }

int Semaphore::tryWait() { return sem_trywait(myHandle); }

// ============= PERIODICNE NITI =============
PeriodicThread::PeriodicThread(time_t period) { this->period = period; }

void PeriodicThread::terminate() { period = 0; }

// ============= KONZOLA =============
char Console::getc() { return ::getc(); }

void Console::putc(char c) { ::putc(c); }