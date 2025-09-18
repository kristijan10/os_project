//#include "../h/syscall_cpp.hpp"
//#include "printing.hpp"
////#include "../h/syscall_c.h"
//
//static volatile bool finishedA = false;
//static volatile bool finishedB = false;
//static volatile bool finishedC = false;
//static volatile int num_of_finished = 0;
//
//class WorkA: public Thread {
//    void workBodyA(void* arg);
////    void mod_fun_A(void* arg);
//public:
//    WorkA():Thread() {}
//
//    void run() override {
//        workBodyA(nullptr);
////        mod_fun_A(nullptr);
//    }
//};
//
//class WorkB: public Thread {
//    void workBodyB(void* arg);
////    void mod_fun_B(void* arg);
//public:
//    WorkB():Thread() {}
//
//    void run() override {
//        workBodyB(nullptr);
////        mod_fun_B(nullptr);
//    }
//};
//
//class WorkC: public Thread {
//    void workBodyC(void* arg);
//public:
//    WorkC():Thread() {}
//
//    void run() override {
//        workBodyC(nullptr);
//    }
//};
//
//void WorkA::workBodyA(void *arg) {
//    //printString("Ovo je nit broj = ");
//    //printInt(getId());
//    int id;
//    for (uint64 i = 0; i < 5; i++) {
//        id = thread_getId();
//        printString("threadA: Myid=");
//        printInt(id);
//        printString("\n");
//    }
//    printString("A finished!\n");
//    finishedA = true;
//    num_of_finished++;
//}
//
//void WorkB::workBodyB(void *arg) {
//    //printString("Ovo je nit broj = ");
//    //printInt(getId());
//    int id;
//    for (uint64 i = 0; i < 5; i++) {
//        id = thread_getId();
//        printString("threadB: Myid=");
//        printInt(id);
//        printString("\n");
//    }
//    printString("B finished!\n");
//    finishedA = true;
//}
//
//void WorkC::workBodyC(void *arg) {
//    //printString("Ovo je nit broj = ");
//    //printInt(getId());
//    int id;
//    for (uint64 i = 0; i < 5; i++) {
//        id = thread_getId();
//        printString("threadC: Myid=");
//        printInt(id);
//        printString("\n");
//    }
//    printString("C finished!\n");
//
//    finishedC = true;
//
//}
//
//void testMainGetId() {
//    Thread* threads[4];
//
//    threads[0] = new WorkA();
//    printString("ThreadA created\n");
//
//    threads[1] = new WorkB();
//    printString("ThreadB created\n");
//
//    threads[2] = new WorkC();
//    printString("ThreadC created\n");
//
//
//    for(int i=0; i<3; i++) {
//        threads[i]->start();
//    }
//
//    while (!(finishedA && finishedB && finishedC)) {
//        Thread::dispatch();
//    }
//
//    for (auto thread: threads) { delete thread; }
//
//}
//
//
//void testMaxThreads(){
//    Thread* threads[20];
//    Thread::setMaxThreadNum(2);
//
//    for(int i=0; i<20; i++) {
//        threads[i] = new WorkA();
//    }
//
//    for(int i=0; i<20; i++) {
//        threads[i]->start();
//    }
//
//    while (num_of_finished != 20) {
//        Thread::dispatch();
//    }
//
//    for (auto thread: threads) { delete thread; }
//
//}