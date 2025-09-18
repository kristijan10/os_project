#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"
#include "printing.hpp"

static bool finishedA = false;
static bool finishedB = false;
static bool finishedC = false;

class gWorkerAMod: public Thread {
public:
    gWorkerAMod():Thread() {}
    //static bool timeForAToPrint;

    void run() override {
        functionAMod(nullptr);
    }

    void functionAMod(void* ptr) {
        for(uint64 i = 0; i < 10; i++) {
            //for(uint64 k = 0; k < 3000000; k++) {
            //    //busy wait!
            //}
            while(!TCB::readyToPrintA) {

            }
            TCB::readyToPrintA = false;
            int idA = getId();
            printString("A: MyID: ");
            printInt(idA);
            printString("\n");

            thread_dispatch();
        }

        finishedA = true;
    }
};

class gWorkerBMod: public Thread {
public:
    gWorkerBMod(): Thread() {}

    void run() override {
        functionBMod(nullptr);
    }

    void functionBMod(void* ptr) {
        for(uint64 i = 0; i< 10; i++) {
            while(!TCB::readyToPrintB) {

            }
            TCB::readyToPrintB = false;
            int idB = getId();
            printString("B: MyID: ");
            printInt(idB);
            printString("\n");

            thread_dispatch();
        }

        finishedB = true;
    }
};

class gWorkerCMod: public Thread {
public:
    gWorkerCMod():Thread() {}

    void run() override {
        functionCMod(nullptr);
    }

    void functionCMod(void* ptr) {
        for(uint64 i = 0; i < 10; i++) {
            while(!TCB::readyToPrintC) {

            }
            TCB::readyToPrintC = false;
            int idC = getId();
            printString("C: MyID: ");
            printInt(idC);
            printString("\n");

            thread_dispatch();
        }

        finishedC = true;
    }
};

void mojTestThreadGetId(){
    Thread *threads[3];

    threads[0] = new gWorkerAMod();
    printString("ThreadA created!\n");

    threads[1] = new gWorkerBMod();
    printString("ThreadB created!\n");

    threads[2] = new gWorkerCMod();
    printString("ThreadC created!\n");

    for(int i=0; i<3; i++) {
        threads[i]->start();
    }

    while(!(finishedA && finishedB && finishedC)) Thread::dispatch();

    printString("\nMain Finished getThreadId() MOD!\n");

    for (auto thread: threads) { delete thread; }
}