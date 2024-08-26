#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/sem.hpp"
#include "../h/allocator.hpp"

bool Riscv::userMode = false;

void Riscv::popSppSpie() {
    mc_sstatus(SSTATUS_SPP);

    asm volatile("csrw sepc, ra");
    asm volatile("sret");
}

void Riscv::handleSupervisorTrap() {
    userMode = false;
    uint64 scause = r_scause();

    uint64 volatile a0, a1, a2, a3;
    asm volatile("mv %0, a3" : "=r" (a3));
    asm volatile("mv %0, a2" : "=r" (a2));
    asm volatile("mv %0, a1" : "=r" (a1));
    asm volatile("mv %0, a0" : "=r" (a0));

    if (scause == 0x0000000000000009UL ||
        scause == 0x0000000000000008UL) {
        // ecall
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        switch (a0) {
            case MEM_ALLOC: {
                void *ret = Allocator::getInstance().mem_alloc((size_t) a1);

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case MEM_FREE: {
                int ret = Allocator::getInstance().mem_free((void *) a1);

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case THREAD_CREATE: {
                auto handle = (TCB **) a1;
                auto body = (TCB::Body) a2;
                auto arg = (void *) a3;

                *handle = TCB::createThread(body, arg);

                int ret = 0;
                if (*handle == nullptr) ret = -11;
                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case THREAD_EXIT: {
                TCB::getRunning()->setFinished(true);
                TCB::dispatch();

                asm volatile("sd %0, 8*10(fp)" : : "r" (0));
                break;
            }
            case THREAD_DISPATCH: {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            }
            case SEM_OPEN: {
                auto handle = (Sem **) a1;
                auto init = (int) a2;

                *handle = Sem::open(init);

                int ret = 0;
                if (*handle == nullptr) ret = -21;
                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case SEM_SIGNAL: {
                auto handle = (Sem *) a1;

                int ret = -24;
                if (handle) ret = handle->signal();

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case SEM_WAIT: {
                auto handle = (Sem *) a1;

                int ret = -23;
                if (handle) ret = handle->wait();

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case SEM_CLOSE: {
                auto handle = (Sem *) a1;

                int ret = -22;
                if (handle) ret = handle->close();

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case SEM_TRYWAIT: {
                auto handle = (Sem *) a1;

                int ret = -26;
                if (handle) ret = handle->trywait();

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case SEM_TIMEDWAIT: {
                auto handle = (Sem *) a1;

                int ret = -26;
                if (handle) ret = handle->trywait();
                // dal se probudio preko timer ili preko

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case TIME_SLEEP: {
                auto t = (uint64) a1;

                TCB::running->setTime(t);
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            }
            case CONSOLE_GETC: {
                char c = __getc();

                asm volatile("sd %0, 8*10(s0)" : : "r" (c));
                break;
            }
            case CONSOLE_PUTC: {
                auto chr = (char) a1;

                __putc(chr);
                break;
            }
            default:
                break;
        }

//        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL) {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        // timer interrupt
        TCB::timeSliceCounter++;
        Scheduler::updateSleep();

        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }

        mc_sip(SIP_SSIP);
        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000009UL) {
// console interrupt
        console_handler();

    } else {
        printStr("-----------\n");
        printStr("SCAUSE: ");
        printInteger(scause);
        printStr("\n");
        printStr("SEPC: "); // gde se desio prekid
        printInteger(r_sepc());

        printStr("\n");
        printStr("STVAL: "); // dodatno objasnjenje interrupt-a
        printInteger(r_stval());

        printStr("\n-----------\n");
        thread_exit();
    }

    userMode = true;
}