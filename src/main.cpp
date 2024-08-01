#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"

int main() {
    TCB *threads[3];
    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];

//    threads[1] = TCB::createThread(workerBodyA);
//    printStr("ThreadA created!\n");

//    threads[2] = TCB::createThread(workerBodyB);
//    printStr("ThreadB created!\n");

    threads[1] = TCB::createThread(workerBodyC);
    printStr("ThreadC created!\n");
    threads[2] = TCB::createThread(workerBodyD);
    printStr("ThreadD created!\n");

    while (!(threads[1]->isFinished() && threads[2]->isFinished()))
        TCB::yield();

    for (auto &thread: threads) delete thread;

    printStr("Finished!\n");
    return 0;
}
