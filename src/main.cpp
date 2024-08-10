#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"

extern void userMain();

void userMainWrapper(void *){
    userMain();
}

int main() {
    printStr("0\n");
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    printStr("1\n");
//    TCB *threads[5];

//TCB *kernel = nullptr, *userThread = nullptr;

    TCB *mainThread = TCB::createThread(nullptr, nullptr);
//    thread_create(&kernel, nullptr, nullptr);
    printStr("mainThread created!\n");
    TCB::running = mainThread;
    printStr("mainThread started!\n");

//    thread_create(&userThread, &userMainWrapper, nullptr);
    TCB *userThread = TCB::createThread(userMainWrapper, nullptr);
    printStr("userMain created!\n");

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    printStr("2\n");

//    userMainWrapper(nullptr);

//    threads[2] = TCB::createThread(workerBodyB);
//    printStr("ThreadB created!\n");
//
//    threads[3] = TCB::createThread(workerBodyC);
//    printStr("ThreadC created!\n");
//
//    threads[4] = TCB::createThread(workerBodyD);
//    printStr("ThreadD created!\n");


//    volatile uint64 i = 3;
    while (!(userThread->isFinished())) {
//        printInteger(i);
//        printStr("\n");
        thread_dispatch();
//        i++;
    }
//        TCB::yield();

    delete userThread;
    delete mainThread;
    printStr("Finished\n");

    return 0;
}