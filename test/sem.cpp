#include "../h/syscall_c.hpp"
#include "./printing.hpp"

static sem_t s;
static int counter = 0;

// ---------- NITI ----------
void producer(void* arg) {
    for(int i = 0; i < 5; i++) {
        sem_wait(s);
        counter++;
        printString("Producer incremented counter\n");
        sem_signal(s);
    }
    thread_exit();
}

void consumer(void* arg) {
    for(int i = 0; i < 5; i++) {
        sem_wait(s);
        counter--;
        printString("Consumer decremented counter\n");
        sem_signal(s);
    }
    thread_exit();
}

void blocked_thread(void* arg) {
    sem_t sem = *(sem_t*)arg;
    int r = sem_wait(sem); // Ovo će blokirati dok se semafor ne zatvori
    printString("Blocked thread unblocked after close, return: ");
    printInt(r, 10, 1);
    printString("\n");
    thread_exit();
}

// ---------- TESTOVI ----------
void test_basic_wait_signal() {
    printString("=== Test 1: Basic wait/signal ===\n");
    sem_open(&s, 1);

    thread_t p, c;
    thread_create(&p, producer, nullptr);
    thread_create(&c, consumer, nullptr);

    // Simulacija raspoređivanja
    for(int i = 0; i < 20; i++) { // dovoljno dispatch-a da se niti završe
        thread_dispatch();
    }

    printString("Counter value: ");
    printInt(counter);
    printString("\n");

    sem_close(s);
}

void test_trywait() {
    printString("=== Test 2: Trywait ===\n");
    sem_t s2;
    sem_open(&s2, 0);

    int res = sem_trywait(s2); // Treba da vrati -2 jer je val = 0
    printString("Trywait result: ");
    printInt(res, 10, 1);
    printString("\n");

    sem_close(s2);
}

void test_close_unblocks() {
    printString("=== Test 3: Close unblocks ===\n");
    sem_t s3;
    sem_open(&s3, 0);

    thread_t t;
    thread_create(&t, blocked_thread, &s3);

    // Raspoređivanje dok nit ne blokira
    thread_dispatch();

    sem_close(s3); // Treba da odblokira nit

    // Još nekoliko dispatch-a da se odblokirana nit izvrši
    for(int i = 0; i < 5; i++) {
        thread_dispatch();
    }
}

// ---------- MAIN ----------
int testSem() {
    test_basic_wait_signal();
    test_trywait();
    test_close_unblocks();
    return 0;
}
