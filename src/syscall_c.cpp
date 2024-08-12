#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../lib/mem.h"

// ============= MEMORIJA =============
void *mem_alloc(size_t size) {
    size_t newSize;
    if (size % MEM_BLOCK_SIZE != 0) {
        newSize = ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE) * MEM_BLOCK_SIZE;
    } else newSize = size;
//
//    asm volatile("mv a1, %0" : : "r" (newSize));
//    asm volatile("mv a0, %0" : : "r" (MEM_ALLOC));
//
//    asm volatile("ecall");
//
//    void *ptr;
//    asm volatile("mv %0, a0" : "=r" (ptr));
//    return ptr;
    return __mem_alloc(newSize);
}

int mem_free(void *ptr) {
//    __asm__ volatile("mv a1, %0" : : "r"(ptr));
//    __asm__ volatile("mv a0, %0" : : "r"(MEM_FREE));
//
//    __asm__ volatile("ecall");
//
//    int retval;
//    __asm__ volatile("mv %0, a0" : "=r"(retval));
//    return retval;
    return __mem_free(ptr);
}

// ============= NITI =============
int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg) {
    asm volatile("mv a3, %0" : : "r" (arg));
    asm volatile("mv a2, %0" : : "r" (start_routine));
    asm volatile("mv a1, %0" : : "r" (handle));
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
//    __asm__ volatile("mv a0, %0" : : "r"(CONSOLE_GETC));
//
//    __asm__ volatile("ecall");
//
//    char c;
//    __asm__ volatile("mv %0, a0" : "=r"(c));
//    return c;
    return __getc();
}

void putc(char c) {
//    __asm__ volatile("mv a1, %0" : : "r"(c));
//    __asm__ volatile("mv a0, %0" : : "r"(CONSOLE_PUTC));
//
//    __asm__ volatile("ecall");
    __putc(c);
}