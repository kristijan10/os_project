#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../lib/console.h"

extern void userMain();

void userMainWrapper(void *) {
    userMain();
}

int main() {
    TCB *mainThread = TCB::createThread(nullptr, nullptr);
    printStr("mainThread created!\n");
    TCB::running = mainThread;

    TCB *userThread = TCB::createThread(userMainWrapper, nullptr);
    printStr("userThread created!\n");

    Riscv::w_sstatus(Riscv::SSTATUS_SIE);
    Riscv::w_stvec((uint64) Riscv::supervisorTrap);

    while (!(mainThread->isFinished() && userThread->isFinished())) {
        thread_dispatch();
    }

    delete userThread;
    delete mainThread;

    printStr("Finished\n");
    return 0;
}