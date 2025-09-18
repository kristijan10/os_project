#include "../h/syscall_c.hpp"
#include "printing.hpp"

// Jednostavan semafor za sinhronizaciju
static sem_t semaphore;
static bool finished[2] = {false};

// Funkcija proizvođača
static void producer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 5; i++) {
        sem_wait(semaphore); // Čekaj semafor
        printString("Proizvođač ");
        printInt(id);
        printString(" proizveo podatak: ");
        printInt(i);
        printString("\n");
        sem_signal(semaphore); // Signaliziraj semafor
        thread_dispatch();
    }

    finished[0] = true;
}

// Funkcija potrošača
static void consumer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 5; i++) {
        sem_wait(semaphore); // Čekaj semafor
        printString("Potrošač ");
        printInt(id);
        printString(" obradio podatak: ");
        printInt(i);
        printString("\n");
        sem_signal(semaphore); // Signaliziraj semafor
        thread_dispatch();
    }

    finished[1] = true;
}

// Funkcija za testiranje semafora
void userMain2() {
    // Inicijalizacija semafora
    sem_open(&semaphore, 1); // Vrednost 1 omogućava sinhronizaciju između dve niti

    // Kreiranje niti
    thread_t producerThread, consumerThread;
    int producerId = 1, consumerId = 2;

    thread_create(&producerThread, producer, &producerId);
    thread_create(&consumerThread, consumer, &consumerId);

    while(!finished[0] && !finished[1]) thread_dispatch();

    // Uništavanje semafora
    sem_close(semaphore);
}
