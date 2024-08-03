#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

extern void userMain();

void userMainWrapper(void *){
    userMain();
}

int main() {
    TCB *threads[5];

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(userMainWrapper, nullptr);
    printStr("userMain created!\n");

//    threads[2] = TCB::createThread(workerBodyB);
//    printStr("ThreadB created!\n");
//
//    threads[3] = TCB::createThread(workerBodyC);
//    printStr("ThreadC created!\n");
//
//    threads[4] = TCB::createThread(workerBodyD);
//    printStr("ThreadD created!\n");

    Riscv::w_sstatus(Riscv::SSTATUS_SIE);
    Riscv::w_stvec((uint64) Riscv::supervisorTrap);

    while (!(threads[1]->isFinished()))
        TCB::yield();

    for (auto &thread: threads) {
        delete thread;
    }
    printStr("Finished\n");

    return 0;
}