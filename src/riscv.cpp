#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/codes.h"
#include "../lib/mem.h"
#include "../h/print.hpp"

void Riscv::popSppSpie() {
    asm volatile("csrw sepc, ra");
    asm volatile("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 scause = r_scause();

    if (scause == 0x0000000000000009UL ||
        scause == 0x0000000000000008UL) {
        // ecall
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();

        uint64 opCode;
        asm volatile("mv %0, a0" : "=r" (opCode));
        printStr("Procitano: ");
        printInteger(opCode);
        printStr("\n");

        switch (opCode) {
            case MEM_ALLOC : {
                uint64 size;
                asm volatile("mv %0, a1" : "=r" (size));

                void *ret = __mem_alloc(size);

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));

                break;
            }
            case MEM_FREE : {
                void *ptr;
                asm volatile("mv %0, a1" : "=r" (ptr));

                int ret = __mem_free(ptr);

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));

                break;
            }
            case THREAD_CREATE: {
                uint64 **handle, start_routine, *arg;

                asm volatile("mv %0, a3" : "=r" (arg));
                asm volatile("mv %0, a2" : "=r" (start_routine));
                asm volatile("mv %0, a1" : "=r" (handle));

                printStr("riscv: THREAD_CREATE\n");
                *handle = (uint64 *) TCB::createThread((TCB::Body) start_routine, (void *) arg);

                uint64 ret = 0;
                if (*handle == nullptr) ret = -11;
                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));

                break;
            }
            case THREAD_EXIT: {
                uint64 ret = 0;

                if (TCB::running != nullptr) {
                    TCB::running->setFinished(true);
                    TCB::dispatch();
                } else ret = -12;

                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));

                break;
            }
            case THREAD_DISPATCH: {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();

                break;
            }
            default:
                break;
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL) {
        // timer interrupt
        TCB::timeSliceCounter++;

        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();

            TCB::timeSliceCounter = 0;
            TCB::dispatch();

            w_sstatus(sstatus);
            w_sepc(sepc);
        }

        mc_sip(SIP_SSIP);
    } else if (scause == 0x8000000000000009UL) {
        // console interrupt
        console_handler();
    } else {}
}