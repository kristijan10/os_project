#ifndef syscall_c_hpp
#define syscall_c_hpp

#include "../lib/hw.h"
#include "codes.h"

// ============= MEMORIJA =============
void *mem_alloc(size_t size);

int mem_free(void *ptr);

// ============= NITI =============
class TCB;

typedef TCB *thread_t;

int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg);

int thread_exit();

void thread_dispatch();

// ============= SEMAFOR =============
class Sem;

typedef Sem *sem_t;

int sem_open(sem_t *handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t handle);

int sem_signal(sem_t handle);

int sem_timedwait(sem_t handle, time_t timeout);

int sem_trywait(sem_t handle);

//typedef unsigned long time_t;
int time_sleep(time_t time);

// ============= KONZOLA =============
const int EOF = -1;

char getc();

void putc(char c);

#endif

// i blokiram i stavim da spavam
// ako prodje vreme samo u skeduler
// ako je iskoriscen signal