#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"

void Riscv::popSppSpie() {
    asm volatile("csrw sepc, ra");
    asm volatile("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 scause = r_scause();

    if(scause == 0x0000000000000009UL){
        // ecall
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();

        TCB::timeSliceCounter = 0;
        TCB::dispatch();

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if(scause == 0x8000000000000001UL){
        // timer interrupt
        TCB::timeSliceCounter++;

        if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()){
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();

            TCB::timeSliceCounter = 0;
            TCB::dispatch();

            w_sstatus(sstatus);
            w_sepc(sepc);
        }

        mc_sip(SIP_SSIP);
    }
    else if(scause == 0x8000000000000009UL){
        // console interrupt
        console_handler();
    } else {}
}