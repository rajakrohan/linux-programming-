#ifndef THREADS_SIGNAL_H
#define THREADS_SIGNAL_H

#include <stdint.h>
#include <stdbool.h>
#include <list.h>

typedef int8_t sigset_t;


enum sig_type{
	SIG_KILL = 0,
	SIG_UBLOCK = 1,
	SIG_USR = 2,
	SIG_CHLD = 3,
	SIG_CPU = 4
};

enum sig_action{
	SIG_IGN = 0,
	SIG_DFL = 1
};

enum procmask_how{
	SIG_BLOCK = 0,
	SIG_UNBLOCK = 1,
	SIG_SETMASK = 2
};


struct thread_signal{
	enum sig_type sig;
    int sender;
    void* reciever; //struct thread is not defined
    struct list_elem elem;
};



int8_t setBit(int8_t n, int8_t bit);

int8_t unsetBit(int8_t n, int8_t bit);

bool checkBit(int8_t n, int8_t bit);

int8_t Signal(enum sig_type s, enum sig_action a);

int8_t kill(int tid, enum sig_type s);

void handleSignals(int8_t);

void KILL_handler(void);
void CPU_handler(int maxlife);
// void UBLOCK_handler(void);
void USR_handler(int x);
void CHILD_handler(void);

int8_t sigemptyset(int8_t *set);
int8_t sigfillset(int8_t *set);
int8_t sigaddset(int8_t *set, enum sig_type s);
int8_t sigdelset(int8_t *set, enum sig_type s);
int8_t sigprocmask(enum procmask_how h, int8_t *set, int8_t *oldset);

void setlifetime(unsigned long long l);

struct list* getRoot(void);

#endif