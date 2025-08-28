#ifndef list_hpp
#define list_hpp

//#include "syscall_c.hpp"

template<typename T>
class List {
private:
    struct Elem {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;
    Elem *iter_curr = head, *iter_prev = nullptr;

public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data) {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data) {
        Elem *elem = new Elem(data, 0);
        if (tail) {
            tail->next = elem;
            tail = elem;
        } else {
            head = tail = elem;
        }
    }

    T *removeFirst() {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekFirst() {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast() {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next) {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast() {
        if (!tail) { return 0; }
        return tail->data;
    }

    T *get(T *t) {
        if (!head) { return 0; }
        Elem *cur = head;
        while (cur && cur != tail) {
            if (cur->data == t) return cur->data;
            cur = cur->next;
        }
        return 0;
    }

    void remove(T *t){
        if(!head) return;

        Elem *cur = head, *prev = nullptr;
        while(cur){
            if(cur->data == t) break;
            prev = cur;
            cur = cur->next;
        }

        if(!cur) return;

        if(!prev){
            head = cur->next;

            if(!head) tail = nullptr;
        } else {
            prev->next = cur->next;

            if(!cur->next) tail = prev;
        }

        delete cur;
    }

    T *iter_get_curr() {
        if(iter_curr) return iter_curr->data;
        return nullptr;
    }

    void iter_next(){
        if(!iter_curr) return;

        iter_prev = iter_curr;
        iter_curr = iter_curr->next;
    }

    void iter_insert_before(T *t) {
        Elem *elem = new Elem(t, 0);

        if (iter_prev) {
            iter_prev->next = elem;
            elem->next = iter_curr;
            iter_prev = elem;
        } else if (iter_curr) {
            head = elem;
            elem->next = iter_curr;
            iter_prev = elem;
        } else {
            head = elem;
            iter_curr = head;
            tail = head;
        }
    }

    void iter_insert_after(T *data) {
        Elem *elem = new Elem(data, 0);
        Elem *next = iter_curr ? iter_curr->next : nullptr;

        if (next) {
            iter_curr->next = elem;
            elem->next = next;
        } else if (iter_curr) {
            iter_curr->next = elem;
            tail = elem;
        } else {
            head = tail = iter_curr = elem;
        }
    }

    void iter_reset() {
        iter_curr = head;
        iter_prev = nullptr;
    }
};

#endif