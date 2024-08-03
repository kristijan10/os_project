#include "../h/syscall_c.h"
#include "../lib/console.h"

// ============= MEMORIJA =============
void *mem_alloc(size_t size) {}

int mem_free(void *ptr) {}

// ============= NITI =============
int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg) {
    asm volatile("mv %0, a3" : : "r" ((uint64) arg));
    asm volatile("mv %0, a2" : : "r" ((uint64) start_routine));
    asm volatile("mv %0, a1" : : "r" ((uint64) handle));
    asm volatile("mv %0, a0" : : "r" ((uint64) THREAD_CREATE));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_exit() {
    asm volatile("mv %0, a0" : : "r" ((uint64) THREAD_EXIT));

    asm volatile("ecall");

    int ret;
    asm volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void thread_dispatch() {
    asm volatile("mv %0, a0" : : "r" ((uint64) THREAD_DISPATCH));

    asm volatile("ecall");
}

// ============= SEMAFOR =============
int sem_open(sem_t *handle, unsigned init) {}

int sem_close(sem_t handle) {}

int sem_wait(sem_t id) {}

int sem_signal(sem_t id) {}

int sem_timedwait(sem_t id, time_t timeout) {}

int sem_trywait(sem_t id) {}

int time_sleep(time_t time){}

// ============= KONZOLA =============
char getc() { return __getc(); }

void putc(char c) { __putc(c); }