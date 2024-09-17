#ifndef sem_hpp
#define sem_hpp

#include "tcb.hpp"
#include "../h/print.hpp"

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
    explicit Sem(int init = 1) : val(init), closed(false), numOfBlocked(0) {}

    int val;
    List<TCB> blocked;
    bool closed;
    int numOfBlocked;
//    int sid;
};

#endif