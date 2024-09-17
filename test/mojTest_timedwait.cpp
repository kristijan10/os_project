#include "mojTest.hpp"
#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

int finnished = 0;
class ModifikacijaThread:public Thread{
private:
    int id;
    Semaphore *sem;
public:
    ModifikacijaThread(int id, Semaphore *sem) : id(id), sem(sem) {}
protected:
    void run() override{
        int i = 3;
        while(i > 0){
            int ret = sem->timedWait(id);
            if(ret == 0){
                printString(">Thread entered ");
                printInt(id, 10, 1);
                printString("\n");
                sleep(id);
                printString("<Thread exited ");
                printInt(id, 10, 1);
                printString("\n");
                sem->signal();
                i--;
            }
            else if(ret == -2){
                printString("**Elapsed time waiting  ");
                printInt(id, 10, 1);
                printString("\n");
            }
            Thread::dispatch();
        }
        finnished++;
    }
};
static Semaphore* waitForAll;
#define NUM 10
void mojTestTimedwait() {
    Thread* threads[NUM];
    waitForAll = new Semaphore(1);
    printString("Test8\n");
    for(int i=0; i<NUM; i++) {
        threads[i] = new ModifikacijaThread(i, waitForAll);
    }
    for(int i=0; i<NUM; i++) {
        threads[i]->start();
    }
    while (finnished != NUM) {
        Thread::dispatch();
    }
    printString("GG\n");
    delete waitForAll;
    for (auto thread: threads) { delete thread; }
}