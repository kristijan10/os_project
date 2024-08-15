#ifndef sem
#define sem

#include "tcb.hpp"

class Sem {
public:
    static Sem *open(int init);

    int wait();

    int signal();

//    int value() const { return val; }

    int close();

protected:
    void block();

    void unblock();

private:
    explicit Sem(int init = 1) : val(init) {}

    ~Sem() { close(); }

    int val;
    List<TCB> blocked;
};

#endif