#include "../h/syscall_cpp.hpp"
#include "../h/allocator.hpp"
#include "../h/riscv.hpp"

// ============= MEMORIJA =============
void *operator new(size_t size) {
    if (!Riscv::userMode) return Allocator::mem_alloc(size);
    return mem_alloc(size);
}

void operator delete(void *ptr) noexcept {
    if (!Riscv::userMode) Allocator::mem_free(ptr);
    else mem_free(ptr);
}

void *operator new[](size_t size) {
    if (!Riscv::userMode) return Allocator::mem_alloc(size);
    return mem_alloc(size);
}

void operator delete[](void *ptr) noexcept {
    if (!Riscv::userMode) Allocator::mem_free(ptr);
    else mem_free(ptr);
}

// ============= NITI =============
Thread::Thread(void (*body)(void *), void *arg) :
        myHandle(nullptr),
        body(body),
        arg(arg) {}

Thread::~Thread() {}

int Thread::start() {
    if (body) thread_create(&myHandle, body, arg);
    else thread_create(&myHandle, wrapper, (void *) this);

    return 0;
}

void Thread::dispatch() { thread_dispatch(); }

int Thread::sleep(time_t time) { return time_sleep(time); }

Thread::Thread() :
        myHandle(nullptr),
        body(nullptr),
        arg(nullptr) {}

void Thread::wrapper(void *thread) {
    ((Thread *) thread)->run();
}

void Thread::join() { if(myHandle) thread_join(&myHandle); }

// ============= SEMAFOR =============
Semaphore::Semaphore(unsigned init) : myHandle(nullptr) { sem_open(&myHandle, init); }

Semaphore::~Semaphore() { sem_close(myHandle); }

int Semaphore::wait() { return sem_wait(myHandle); }

int Semaphore::signal() { return sem_signal(myHandle); }

int Semaphore::timedWait(time_t time) { return sem_timedwait(myHandle, time); }

int Semaphore::tryWait() { return sem_trywait(myHandle); }

// ============= PERIODICNE NITI =============
PeriodicThread::PeriodicThread(time_t period) { this->period = period; }

void PeriodicThread::terminate() { period = 0; }

void PeriodicThread::run() {
    while (period) {
        sleep(period);
        periodicActivation();
    }

    thread_exit();
}

// ============= KONZOLA =============
char Console::getc() { return ::getc(); }

void Console::putc(char c) { ::putc(c); }