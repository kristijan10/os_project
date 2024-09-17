#include "../h/syscall_cpp.hpp"
#include "mojTest.hpp"

#include "printing.hpp"

bool finishedA = false;
bool finishedB = false;

class mWorkerA: public Thread {
    void mod_workerBodyA(void* arg);
public:
    mWorkerA():Thread() {}

    void run() override {
        mod_workerBodyA(nullptr);
    }
};

class mWorkerB: public Thread {
    void mod_workerBodyB(void* arg);
public:
    mWorkerB():Thread() {}

    void run() override {
        mod_workerBodyB(nullptr);
    }
};

Thread* mod_threads[2];

void mWorkerA::mod_workerBodyA(void *arg) {
    for (uint64 i = 0; i < 10; i++) {
        printString("A: i="); printInt(i); printString("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }

    printString("A finished!\n");
    finishedA = true;
}

void mWorkerB::mod_workerBodyB(void *arg) {
    for (uint64 i = 0; i < 16; i++) {
        printString("B: i="); printInt(i); printString("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    printString("B finished!\n");
    finishedB = true;
    thread_dispatch();
}

void mojTestJoin() {

    mod_threads[0] = new mWorkerA();
    printString("ThreadA created\n");

    mod_threads[1] = new mWorkerB();
    printString("ThreadB created\n");

    for(int i=0; i<2; i++) {
        mod_threads[i]->start();
    }

    mod_threads[0]->join();
    mod_threads[1]->join();

    for (auto thread: mod_threads) { delete thread; }
}
