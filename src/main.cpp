#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/sem.hpp"
#include "../h/allocator.hpp"

void userMain();
void testSemaphores();

void userMainWrapper(void *) {
    userMain();
}

int main() {
    Allocator::init();

    TCB *threads[2];

    // postavljanje supervisor trap handlera
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    // omogućavanje interapt-a
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    // kreiranje glavnog thread-a
    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    // kreiranje korisničkog thread-a
    threads[1] = TCB::createThread(userMainWrapper, nullptr);

    // dispatcher dok se korisnički thread ne završi
    while (!threads[1]->isFinished()) thread_dispatch();

    delete threads[1];

    return 0;
}
