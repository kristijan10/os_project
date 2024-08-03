#include "../h/syscall_cpp.hpp"
#include "../lib/mem.h"
#include "../h/scheduler.hpp"
#include "../h/print.hpp"

// ============= MEMORIJA =============
void *operator new(size_t size) {
//    printStr("_cpp: new\n");
    return __mem_alloc(size); }

void operator delete(void *ptr) noexcept {
//    printStr("_cpp: delete\n");
    __mem_free(ptr); }


// ============= NITI =============
Thread::Thread(void (*body)(void *), void *arg) :
        myHandle(nullptr),
        body(body),
        arg(arg) {
    thread_create(&myHandle, body, arg);
}

Thread::~Thread() noexcept {}

int Thread::start() {
//    if(body != nullptr) return thread_create(&myHandle, body, arg);
//    return thread_create(&myHandle, runWrapper, this);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Thread::Thread() : myHandle(nullptr), body(nullptr), arg(this) {
    thread_create(&myHandle, runWrapper, this);
}

void Thread::runWrapper(void *ptr) {
    if(ptr) ((Thread *)ptr)->run();
}

// ============= SEMAFOR =============
Semaphore::Semaphore(unsigned int init) : myHandle(nullptr) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() { sem_close(myHandle); }

int Semaphore::wait() { return sem_wait(myHandle); }

int Semaphore::signal() { return sem_signal(myHandle); }

int Semaphore::timedWait(time_t time) { return sem_timedwait(myHandle, time); }

int Semaphore::tryWait() { return sem_trywait(myHandle); }

// ============= PERIODICNE NITI =============
PeriodicThread::PeriodicThread(time_t period) : period(period) {}

void PeriodicThread::terminate() { period = 0; }

// ============= KONZOLA =============
void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}