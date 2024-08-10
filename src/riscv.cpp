#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/codes.h"
#include "../h/print.hpp"
#include "../lib/mem.h"

void Riscv::popSppSpie() {
    asm volatile("csrw sepc, ra");
//    mc_sstatus(SSTATUS_SPP);
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

        switch(opCode){
            case MEM_ALLOC: {
                size_t numOfBlocks;
                asm volatile("mv %0, a1" : "=r" (numOfBlocks));

                void *ptr = __mem_alloc(numOfBlocks);
                asm volatile("sd %0, 8*10(fp)" : : "r" (ptr));
                break;
            }
            case MEM_FREE: {
                void *ptr;
                asm volatile("mv %0, a1" : "=r" (ptr));

                int ret = __mem_free(ptr);
                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case THREAD_CREATE: {
                void **handle, *arg;
                TCB::Body body;

                asm volatile("mv %0, a3" : "=r" (arg));
                asm volatile("mv %0, a2" : "=r" (body));
                asm volatile("mv %0, a1" : "=r" (handle));

                *handle = TCB::createThread(body, arg);

                uint64 ret = 0;
                if(*handle == nullptr) ret = -11;
                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case THREAD_EXIT: {}
            case THREAD_DISPATCH: {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            }
            case CONSOLE_GETC: {
                char c = __getc();

                asm volatile("sd %0, 8*10(fp)" : : "r" (c));
                break;
            }
            case CONSOLE_PUTC: {
                char c;
                asm volatile("mv %0, a1" : "=r" (c));

                __putc(c);
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
    }
}