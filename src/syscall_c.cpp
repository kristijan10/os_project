#include "../h/syscall_c.h"
#include "../lib/mem.h"
#include "../lib/console.h"
#include "../h/print.hpp"

// ============= MEMORIJA =============
void *mem_alloc(size_t size) {
    printStr("_c: mem_alloc\n");
    uint64 numOfBlocks = size / MEM_BLOCK_SIZE + (size % MEM_BLOCK_SIZE ? 1 : 0) + 1;

    asm volatile("mv a1, %0" : : "r" (numOfBlocks));
    asm volatile("mv a0, %0" : : "r" ((uint64) MEM_ALLOC));

    asm volatile("ecall");

    void *ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int mem_free(void *ptr) {
    return __mem_free(ptr);
}

// ============= NITI =============
int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg) {
    asm volatile("mv a3, %0" : : "r" ((uint64) arg));
    asm volatile("mv a2, %0" : : "r" ((uint64) start_routine));
    asm volatile("mv a1, %0" : : "r" ((uint64) handle));
    asm volatile("mv a0, %0" : : "r"((uint64) THREAD_CREATE));

    printStr("thread_create\na0=");
    printInteger(THREAD_CREATE);
    printStr("\n");
    asm volatile("ecall");

    uint64 ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return (int) ret;
}

int thread_exit() {
    asm volatile("mv a0, %0" : : "r" ((uint64) THREAD_EXIT));

    asm volatile("ecall");

    uint64 ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return (int) ret;
}

void thread_dispatch() {
    asm volatile("mv a0, %0" : : "r" ((uint64) THREAD_DISPATCH));

    asm volatile("ecall");
}

// ============= SEMAFOR =============
int sem_open(sem_t *handle, unsigned init) { return 0; }

int sem_close(sem_t handle) { return 0; }

int sem_wait(sem_t id) { return 0; }

int sem_signal(sem_t id) { return 0; }

int sem_timedwait(sem_t id, time_t timeout) { return 0; }

int sem_trywait(sem_t id) { return 0; }

int time_sleep(time_t time) { return 0; }

// ============= KONZOLA =============
char getc() {
//    asm volatile("mv a0, %0" : : "r" ((uint64) CONSOLE_GETC));
//
//    asm volatile("ecall");
//
//    uint64 ret;
//    asm volatile("mv %0, a0" : "=r" (ret));
//    return (char) ret;
    return __getc();
}

void putc(char c) {
//    asm volatile("mv a1, %0" : : "r" ((uint64) c));
//    asm volatile("mv a0, %0" : : "r" ((uint64) CONSOLE_PUTC));
//
//    asm volatile("ecall");
    __putc(c);
}