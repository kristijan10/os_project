#ifndef sem
#define sem

#include "tcb.hpp"
#include "../h/print.hpp"

//static int SID = 0;

class Sem {
public:
    static Sem *open(int init);

    int wait();

    int signal();

    int close();

    ~Sem() { close(); }

protected:
    void block();

    void unblock();

private:
    explicit Sem(int init = 1) : val(init), closed(false), numOfBlocked(0) {
//        printStr("Otvoren semafor\n");
//        printInteger(sid);
//        printStr("\n");
    }

    int val;
    List<TCB> blocked;
    bool closed;
    int numOfBlocked;
//    int sid;
};

#endif