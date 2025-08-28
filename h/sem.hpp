#ifndef sem_hpp
#define sem_hpp

//#include "tcb.hpp"
#include "list.hpp"
#include "../lib/hw.h"
class TCB;

//static int SID = 0;

class Sem {
public:
    static Sem *open(int init);

    int wait();

    int signal();

    int close();

    int trywait();

    int timedwait(time_t timeout);

    ~Sem() { close(); }

protected:
    void block();

    void unblock();

private:
    explicit Sem(int init = 1) : val(init), closed(false) {}

    int val;
    List<TCB> blocked;
    bool closed;
//    int numOfBlocked;
//    int sid;
};

#endif