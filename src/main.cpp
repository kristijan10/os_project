#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/sem.hpp"

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

    gotoUser();

    thread_t user;
    thread_create(&user, userMainWrapper, nullptr);

//    threads[1] = TCB::createThread(userMainWrapper, nullptr);

    while (!user->isFinished()) thread_dispatch();

    delete user;
    return 0;
}