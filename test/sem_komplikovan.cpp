#include "../h/tcb.hpp"
#include "../h/sem.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

static volatile int sharedCounterWait = 0;
static volatile int sharedCounterTry = 0;
static volatile int sharedCounterSignal = 0;
static volatile bool finished[3] = {false, false, false};
static sem_t semWait;
static sem_t semTry;
static sem_t semSignal;

// ===== TEST WAIT =====
static void workerWait(void *arg) {
    for (int i = 0; i < 5; i++) {
        sem_wait(semWait);
        printString("Worker WAIT acquired sem\n");
        sharedCounterWait++;
        thread_dispatch();
        sem_signal(semWait);
    }
    finished[0] = true;
}

void testWait() {
    thread_t thread;
    sem_open(&semWait, 1);
    thread_create(&thread, workerWait, nullptr);

    while (!finished[0]) thread_dispatch();

    printString("Final sharedCounterWait: ");
    printInt(sharedCounterWait);
    printString("\n");
    sem_close(semWait);
}

// ===== TEST TRYWAIT =====
static void workerTry(void *arg) {
    for (int i = 0; i < 10; i++) {
        if (sem_trywait(semTry) == 0) {
            printString("Worker TRYWAIT got sem\n");
            sharedCounterTry++;
            sem_signal(semTry);
        } else {
            printString("Worker TRYWAIT couldn't get sem\n");
        }
        thread_dispatch();
    }
    finished[1] = true;
}

void testTryWait() {
    thread_t thread;
    sem_open(&semTry, 1);
    thread_create(&thread, workerTry, nullptr);

    while (!finished[1]) thread_dispatch();

    printString("Final sharedCounterTry: ");
    printInt(sharedCounterTry);
    printString("\n");
    sem_close(semTry);
}

// ===== TEST SIGNAL =====
static void workerSignal(void *arg) {
    for (int i = 0; i < 5; i++) {
        printString("Worker SIGNAL is signaling sem\n");
        sem_signal(semSignal);
        sharedCounterSignal++;
        thread_dispatch();
    }
    finished[2] = true;
}

void testSignal() {
    thread_t thread;
    sem_open(&semSignal, 0);  // start sa 0 da testira signal
    thread_create(&thread, workerSignal, nullptr);

    while (!finished[2]) thread_dispatch();

    printString("Final sharedCounterSignal: ");
    printInt(sharedCounterSignal);
    printString("\n");
    sem_close(semSignal);
}

// ===== FUNKCIJA KOJA POZIVA SVE TESTOVE =====
void testSemaphores() {
    testWait();
thread_dispatch();
    testTryWait();
//    testSignal();
    printString("All semaphore tests finished\n");
}
