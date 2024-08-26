#include "mojTest.hpp"
#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

sem_t sem;
void f(void* arg){
    printString("ASD\n");
    time_sleep(5);
    sem_close(sem);
    printString("ASD\n");
}
class Test : public PeriodicThread{
public:
    Test(time_t t) : PeriodicThread(t) {}
    void periodicActivation() override{
        printString("SAD\n");
    }
};
#define NUM 10
void randomTest(){
    printString("Test9\n");
    Test *pt = new Test(10);
    pt->start();

    time_sleep(200);
    printString("Terminating\n");
    pt->terminate();
    while(1);
    /*thread_t t;
    thread_create(&t, f, sem);

    sem_open(&sem,0);

    int ret = sem_wait(sem);
    if(ret == -1){
    printString("ret ");
    printInt(ret);
    printString("\n");
    }*/
    printString("GG\n");
}