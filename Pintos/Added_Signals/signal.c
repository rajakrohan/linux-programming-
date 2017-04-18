#include <stdio.h>
#include <list.h>
#include "threads/signal.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include <debug.h>

struct list sig_list; //root signal of list, does not contain any signal info



int8_t setBit(int8_t n, int8_t bit){
	if(bit < 0 || bit > 7) return n;
	int8_t num = 1;
	num = num<<bit;
	n = n|num;
	return n;
}

int8_t unsetBit(int8_t n, int8_t bit){
	if(bit < 0 || bit > 7) return n;
	int8_t num = 1;
	num = num<<bit;
	num = ~num;
	n = n&num;
	return n;
}

bool checkBit(int8_t n, int8_t bit){
	if(bit < 0 || bit > 7) return 0;
	int8_t num = 1;
	num = num<<bit;
	n = n&num;
	if(n == 0) return 0;
	return 1;
}

int8_t Signal(enum sig_type s, enum sig_action a){
	struct thread *t = thread_current();
	if(a == SIG_DFL){
		t->sigign = setBit(t->sigign, s);
	}else if(a == SIG_IGN){
		t->sigign = unsetBit(t->sigign, s);
	}else{
		return -1;
	}
	return 0;
}

int8_t kill(int tid, enum sig_type s){
	if(s == SIG_KILL || s == SIG_UBLOCK || s == SIG_USR){
		struct thread* st = thread_current();

		enum intr_level old_level;
		old_level = intr_disable ();

		struct thread* dt = get_thread(tid);

		if(dt == NULL) return -1;
		
		if(s == SIG_KILL){
			
			if(dt->parent != st->tid){
				return -1;
			}
		}

		// struct thread_signal *x = &sig_list;
		
		

		// while(x->next != NULL){
		// 	x = x->next;
			
		// 	if(x->reciever == (void*)dt && x->sig == s){
		// 		x->sender = st->tid;
		// 		return 0;
		// 	}
		// }

		

		// x->next = (struct thread_signal*)malloc(sizeof(struct thread_signal));
		// x = x->next;
		// x->sig = s;
		// x->sender = st->tid;
		// x->reciever = (void*)dt;
		// x->next = NULL;

		bool found = 0;

		struct list_elem *e;
	    for (e = list_begin (&sig_list); (e != list_end (&sig_list))&&(!found); e = list_next (e)){
	        
	        struct thread_signal *ts = list_entry (e, struct thread_signal, elem);
	      
	        if(ts->reciever == (void*)dt && ts->sig == s){
	        	ts->sender = st->tid;
	        	found = 1;
	        }

	    }

	    if(!found){
	    	struct thread_signal *x = (struct thread_signal*)malloc(sizeof(struct thread_signal));
	    	x->sig = s;
	    	x->sender = st->tid;
	    	x->reciever = (void*)dt;
	    	list_push_back (&sig_list, &x->elem);
	    }


		intr_set_level (old_level);
		

		return 0;
	}
	return -1;	
}

void KILL_handler(void){
	printf("\nSIG_KILL\n");
	thread_exit();
}

void CPU_handler(int maxlife){
	printf("\nMax_lifetime = %d\n", maxlife);
	thread_exit();
}

void USR_handler(int x){
	printf("\nSIG_USR from - %d", x);
}

void CHILD_handler(void){
	struct thread *t = thread_current();
	t->dchild++;
	printf("\nSIG_CHLD - %d created - %d alive \n", t->child, (t->child)-(t->dchild));
}

// void UBLOCK_handler(void){
// 	//unblock
	
// }

void handleSignals(int8_t scpu){


	struct thread *t = thread_current();

	int8_t sigd = 0;
	int sender = 0;

	

	// struct thread_signal *y = &sig_list;

	// while(y != NULL && y->next != NULL){
		
	// 	struct thread_signal *x = y->next;


		// struct thread *ttt = (struct thread*) x->reciever;

		// if(x->sig == SIG_UBLOCK){
			
		// 	if(checkBit(ttt->sigmask, SIG_UBLOCK) == 0){
		// 		if(checkBit(ttt->sigign, SIG_UBLOCK) == 1){
		// 		  if(ttt->status == THREAD_BLOCKED){
		// 		    thread_unblock(ttt);
		// 		  }
		// 		  y->next = x->next;
		// 		  free(x);
		// 		}
		// 	}
		  
		// }else{
		// 	if(ttt == t){
		// 		if(x->sig == SIG_KILL){
		// 			sigd = setBit(sigd, x->sig);
		// 			y->next = x->next;
		// 			free(x);
		// 		}else if(checkBit(t->sigmask, x->sig) == 0){
		// 			sigd = setBit(sigd, x->sig);
		// 			y->next = x->next;
		// 			if(x->sig == SIG_USR){
		// 				sender = x->sender;
		// 			}
		// 			free(x);
		// 		}
				
		// 	}
		// }
	// 	y = y->next;
	// }

	struct list_elem *e;
    for (e = list_begin (&sig_list); e != list_end (&sig_list); e = list_next (e)){
        
        struct thread_signal *x = list_entry (e, struct thread_signal, elem);

      	struct thread *ttt = (struct thread*) x->reciever;

		if(x->sig == SIG_UBLOCK){
			
			if(checkBit(ttt->sigmask, SIG_UBLOCK) == 0){
				if(checkBit(ttt->sigign, SIG_UBLOCK) == 1){
				  if(ttt->status == THREAD_BLOCKED){
				    thread_unblock(ttt);
				  }
				  list_remove (&x->elem);
				  
				}
			}
		  
		}else{
			if(ttt == t){
				if(x->sig == SIG_KILL){
					sigd = setBit(sigd, x->sig);
					list_remove (&x->elem);
					
				}else if(checkBit(t->sigmask, x->sig) == 0){
					sigd = setBit(sigd, x->sig);
					
					if(x->sig == SIG_USR){
						sender = x->sender;
					}
					
					list_remove (&x->elem);
					
				}
				
			}
		}

    }
	
	if(checkBit(t->sigmask, SIG_CPU) == 0){
		if(scpu){
			sigd = setBit(sigd, SIG_CPU);
		}
	}

	
	int8_t calls = (sigd)&(t->sigign);

	//if(calls != 0) printf("\n\n%d = %d %d %d\n\n", calls, t->signal.signal, t->signal.mask ,t->sigmask);

	
	
	if(checkBit(calls, SIG_USR) == 1){
		USR_handler(sender);
		
	}



	if(checkBit(calls, SIG_CHLD) == 1){
		
		CHILD_handler();
	}



	if(checkBit(calls, SIG_CPU) == 1){
		CPU_handler(t->maxlifetime);
	}
	

	if(checkBit(sigd, SIG_KILL) == 1){
		KILL_handler();
	}
	
}

int8_t sigemptyset(int8_t *set){
	if(set == NULL) return 0;
	*set = 0;
	return 0;
}
int8_t sigfillset(int8_t *set){
	if(set == NULL) return 0;
	*set = 0;
	*set = ~(*set);
	return 0;
}
int8_t sigaddset(int8_t *set, enum sig_type s){
	if(s < 0 || s > 4) return -1;
	if(set == NULL) return 0;
	*set = setBit(*set, s);
	return 0;
}
int8_t sigdelset(int8_t *set, enum sig_type s){
	if(s < 0 || s > 4) return -1;
	if(set == NULL) return 0;
	*set = unsetBit(*set, s);
	return 0;
}


int8_t sigprocmask(enum procmask_how h, int8_t *set, int8_t *oldset){
	struct thread *t = thread_current();
	if(oldset != NULL) *oldset = t->sigmask;
	if(set == NULL) return 0;
	int8_t op = *set;
	if(h == SIG_BLOCK){
		t->sigmask = t->sigmask | op;
	}else if(h == SIG_UNBLOCK){
		op = ~op;
		t->sigmask = t->sigmask & op;
	}else if(h == SIG_SETMASK){
		t->sigmask = op;
	}else{
		return -1;
	}
	return 0;
}

void setlifetime(unsigned long long x){
	struct thread *t = thread_current();
	t->maxlifetime = x;
}

struct list* getRoot(void){
	return &sig_list;
}