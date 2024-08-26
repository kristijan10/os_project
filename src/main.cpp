#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/sem.hpp"
#include "../h/allocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"

extern void userMain();

void userMainWrapper(void *) {
    userMain();
}

int main() {
    TCB *threads[2];

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::setRunning(threads[0]);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    threads[1] = TCB::createThread(userMainWrapper, nullptr);

    while (!threads[1]->isFinished()) thread_dispatch();

    delete threads[1];
    return 0;
}