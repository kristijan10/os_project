#include "../h/tcb.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

static volatile int sharedCounter = 0;
static volatile bool finished[6] = {false, false, true, true, false, false};

// Worker A/B: inkrementuje lokalni i deljeni brojač
static void workerAB(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 10 + id; i++) {
        printString(id == 0 ? "A: " : "B: ");
        printInt(i);
        printString("\n");

        sharedCounter++;
        thread_dispatch();
    }
    finished[id] = true;
}

// Worker C/D: pokušava da čita supervisor registar iz U-mode
//static void workerCD(void* arg) {
//    int id = *(int*)arg;
//    printString(id == 2 ? "C trying sstatus\n" : "D trying sstatus\n");
//    uint64 sstatusVal = Riscv::r_sstatus(); // ovo bi trebalo da baci exception u U-mode
//    printString("Read sstatus: 0x"); printInt(sstatusVal); printString("\n");
//    finished[id] = true;
//    thread_dispatch();
//}

// Worker E/F: Fibonacci i deljeni zbir
static uint64 fibonacci(uint64 n) {
    if (n < 2) return n;
    if (n % 5 == 0) thread_dispatch();
    return fibonacci(n-1) + fibonacci(n-2);
}

static void workerEF(void* arg) {
    int id = *(int*)arg;
    uint64 result = fibonacci(12 + id);
    printString(id == 4 ? "E fib: " : "F fib: ");
    printInt(result);
    printString("\n");
    sharedCounter += result;
    finished[id] = true;
    thread_dispatch();
}

void complexTCBTest() {
    thread_t threads[6];
    int ids[6] = {0,1,2,3,4,5};

    // Kreiranje niti
    thread_create(&threads[0], workerAB, &ids[0]);
    thread_create(&threads[1], workerAB, &ids[1]);
//    thread_create(&threads[2], workerCD, &ids[2]);
//    thread_create(&threads[3], workerCD, &ids[3]);
    thread_create(&threads[4], workerEF, &ids[4]);
    thread_create(&threads[5], workerEF, &ids[5]);

    // Dispatcher loop
    while (!(finished[0] && finished[1] && finished[2] && finished[3] && finished[4] && finished[5])) {
        thread_dispatch();
    }

    printString("Final sharedCounter: ");
    printInt(sharedCounter);
    printString("\nKernel finished\n");
}
