#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/allocator.hpp"

// ============= MEMORIJA =============
void *mem_alloc(size_t size) {
    size_t newSize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    asm volatile("mv a1, %0" : : "r" (newSize));
    asm volatile("mv a0, %0" : : "r" (MEM_ALLOC));

    asm volatile("ecall");

    void *ptr;
    asm volatile("mv %0, a0" : "=r" (ptr));
    return ptr;
}

int mem_free(void *ptr) {
//    if (Riscv::userMode) {
    asm volatile("mv a1, %0" : : "r"(ptr));
    asm volatile("mv a0, %0" : : "r"(MEM_FREE));

    asm volatile("ecall");

    int retval;
    asm volatile("mv %0, a0" : "=r"(retval));
    return retval;
//    } else return Allocator::getInstance().mem_free(ptr);
}

// ============= NITI =============
int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg) {
    asm volatile("mv a3, %0" : : "r" ((uint64) arg));
    asm volatile("mv a2, %0" : : "r" ((uint64) start_routine));
    asm volatile("mv a1, %0" : : "r" ((uint64) handle));
    asm volatile("mv a0, %0" : : "r" (THREAD_CREATE));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_exit() {
    asm volatile("mv a0, %0" : : "r" (THREAD_EXIT));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void thread_dispatch() {
    asm volatile("mv a0, %0" : : "r" (THREAD_DISPATCH));

    asm volatile("ecall");
}

void thread_join(thread_t *handle){
    asm volatile("mv a1, %0" : : "r" ((uint64) handle));
    asm volatile("mv a0, %0" : : "r" (THREAD_JOIN));

    asm volatile("ecall");
}

// ============= SEMAFOR =============
int sem_open(sem_t *handle, unsigned init) {
    asm volatile("mv a2, %0" : : "r" (init));
    asm volatile("mv a1, %0" : : "r" (handle));
    asm volatile("mv a0, %0" : : "r" (SEM_OPEN));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_close(sem_t handle) {
    asm volatile("mv a1, %0" : : "r" (handle));
    asm volatile("mv a0, %0" : : "r" (SEM_CLOSE));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_wait(sem_t handle) {
    asm volatile("mv a1, %0" : : "r" (handle));
    asm volatile("mv a0, %0" : : "r" (SEM_WAIT));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_signal(sem_t handle) {
    asm volatile("mv a1, %0" : : "r" (handle));
    asm volatile("mv a0, %0" : : "r" (SEM_SIGNAL));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_timedwait(sem_t id, time_t timeout) {
    asm volatile("mv a2, %0" : : "r" (timeout));
    asm volatile("mv a1, %0" : : "r" (id));
    asm volatile("mv a0, %0" : : "r" (SEM_TIMEDWAIT));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_trywait(sem_t id) {
    asm volatile("mv a1, %0" : : "r" (id));
    asm volatile("mv a0, %0" : : "r" (SEM_TRYWAIT));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int time_sleep(time_t time) {
    asm volatile("mv a1, %0" : : "r" (time));
    asm volatile("mv a0, %0" : : "r" (TIME_SLEEP));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

// ============= KONZOLA =============
char getc() {
    asm volatile("mv a0, %0" : : "r"(CONSOLE_GETC));

    asm volatile("ecall");

    char c;
    asm volatile("mv %0, a0" : "=r"(c));
    return c;
//    return __getc();
}

void putc(char c) {
    asm volatile("mv a1, %0" : : "r"(c));
    asm volatile("mv a0, %0" : : "r"(CONSOLE_PUTC));

    asm volatile("ecall");
//    __putc(c);
}