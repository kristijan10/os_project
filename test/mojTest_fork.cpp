#include "mojTest.hpp"
#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

void forkexample() {
    int x = 1;
    int p = x;
    if (p < 0) {
        printString("Nisam uspeo da napravim!");
        return;
    } else if (p == 0) {
        printString("Child has x = ");
        printInt(++x);
    } else {
        printString("Parent has x = ");
        printInt(--x);
    }
    printString("\n");
}

void mojTestFork() {
    forkexample();
    printString("GG\n");
}