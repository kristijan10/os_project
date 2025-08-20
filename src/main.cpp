#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/sem.hpp"
#include "../h/allocator.hpp"

extern void userMain();

void userMainWrapper(void *) {
    userMain();
}

int main() {
    Allocator::init();

    TCB *threads[2];

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    threads[1] = TCB::createThread(userMainWrapper, nullptr);

    while (!threads[1]->isFinished()) TCB::yield();

//    delete threads[1];
    return 0;
}