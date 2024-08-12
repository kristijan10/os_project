#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"

extern void userMain();

void userMainWrapper(void *) {
    userMain();
}

int main() {
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB *main = TCB::createThread(nullptr, nullptr);
    TCB::setRunning(main);

    TCB *user = TCB::createThread(userMainWrapper, nullptr);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!user->isFinished()) TCB::yield();

    printStr("Finished!\n");
    delete user;
    delete main;

    return 0;
}