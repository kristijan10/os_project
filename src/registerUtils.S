//.global _ZN3TCB13pushRegistersEv
//.type _ZN3TCB13pushRegistersEv, @function
//_ZN3TCB13pushRegistersEv:
.global pushRegisters
.type pushRegisters, @function
pushRegisters:
    addi sp, sp, -256
    .irp index, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr
    ret

//.global _ZN5Riscv12popRegistersEv
//.type _ZN5Riscv12popRegistersEv, @function
//_ZN5Riscv12popRegistersEv:
.global popRegisters
.type popRegisters, @function
popRegisters:
    .irp index, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    ret