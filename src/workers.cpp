#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/print.hpp"


static uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) TCB::yield();
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC() {
    uint8 i = 0;
    for (; i < 3; i++) {
        printStr("C: i=");
        printInteger(i);
        printStr("\n");
    }

    printStr("C: yield\n");
    __asm__ ("li t1, 7");
    TCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printStr("C: t1=");
    printInteger(t1);
    printStr("\n");

    uint64 result = fibonacci(12);
    printStr("C: fibonaci=");
    printInteger(result);
    printStr("\n");

    for (; i < 6; i++) {
        printStr("C: i=");
        printInteger(i);
        printStr("\n");
    }
}

void workerBodyD() {
    uint8 i = 10;
    for (; i < 13; i++) {
        printStr("D: i=");
        printInteger(i);
        printStr("\n");
    }

    printStr("D: yield\n");
    __asm__ ("li t1, 5");
    TCB::yield();

    uint64 result = fibonacci(16);
    printStr("D: fibonaci=");
    printInteger(result);
    printStr("\n");

    for (; i < 16; i++) {
        printStr("D: i=");
        printInteger(i);
        printStr("\n");
    }

    TCB::running->setFinished(true);
    TCB::yield();
}

void workerBodyA() {
    for (uint64 i = 0; i < 10; i++) {
        printStr("A: i=");
        printInteger(i);
        printStr("\n");
        for (uint64 j = 0; j < 10000; j++)
            for (uint64 k = 0; k < 30000; k++);
    }
}

void workerBodyB() {
    for (uint64 i = 0; i < 10; i++) {
        printStr("B: i=");
        printInteger(i);
        printStr("\n");
        for (uint64 j = 0; j < 10000; j++)
            for (uint64 k = 0; k < 30000; k++);
    }
}
