#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/codes.h"
#include "../h/print.hpp"
#include "../lib/mem.h"
#include "../h/syscall_c.h"

void Riscv::popSppSpie() {
    asm volatile("csrw sepc, ra");
//    mc_sstatus(SSTATUS_SPP);
    asm volatile("sret");
}

void Riscv::handleSupervisorTrap() {
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

//        uint64 opCode;
//        asm volatile("mv %0, a0" : "=r" (a0));

        switch (a0) {
//            case MEM_ALLOC: {
//                size_t size;
//                asm volatile("mv %0, a1" : "=r" (size));
//
//                void *ptr = __mem_alloc(size);
//                asm volatile("sd %0, 8*10(fp)" : : "r" (ptr));
//                break;
//            }
//            case MEM_FREE: {
//                void *ptr;
//                asm volatile("mv %0, a1" : "=r" (ptr));
//
//                int ret = __mem_free(ptr);
//                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
//                break;
//            }
            case THREAD_CREATE: {
                TCB **handle = (TCB **) a1;
                TCB::Body body = (TCB::Body) a2;
                void *arg = (void *) a3;


                *handle = TCB::createThread(body, arg);

                int ret = 0;
                if (*handle == nullptr) ret = -11;
                asm volatile("sd %0, 8*10(fp)" : : "r" (ret));
                break;
            }
            case THREAD_EXIT: {
                break;
            }
            case THREAD_DISPATCH: {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            }
//            case CONSOLE_GETC: {
//                char c = __getc();
//
//                asm volatile("sd %0, 8*10(s0)" : : "r" (c));
//                break;
//            }
//            case CONSOLE_PUTC: {
//                char chr;
//                asm volatile("mv %0, a1" : "=r" (chr));
//
//                __putc(chr);
//                break;
//            }
            default:
                break;
        }

        w_sepc(sepc);
        w_sstatus(sstatus);
    } else if (scause == 0x8000000000000001UL) {
        // timer interrupt
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
//        TCB::timeSliceCounter++;

        mc_sip(SIP_SSIP);
//        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
//            TCB::timeSliceCounter = 0;
//            TCB::dispatch();
//        }

        w_sepc(sepc);
        w_sstatus(sstatus);
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