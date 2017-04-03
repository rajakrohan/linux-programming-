Main Scheduling Functions
1. thread_init : Initializes threading system ( both ready list, and a list containing all
threads)
2. thread_start : Starts preemptive scheduling by creating idle thread and enabling
interrupts
3. Function thread_tick : called by timer interrupt handler (update stats and waiting
time for threads, if current running thread should yield the CPU, it calls
intr_yield_on_return() function)
4. Function thread_block : blocks current thread
5. Function thread_unblock : unblock a blocked thread
6. Function thread_exit : destroy current thread
7. Function thread_yield : current thread is put into ready queue and schedule() is
called
8. Function next_thread_to_run : returns next thread to be scheduled
9. Function schedule : make next_thread_to_run() current thread and calls
thread_schedule_tail(prev thread)
10. Function thread_schedule _tail : destroy prev thread if it is dying , set status of
current thread to running and activate current thread’s page tables
PintOS Scheduling System
1. Scheduling Policy​ : Round Robin on one ready queue.
2. Each thread can be in one of four states THREAD_RUNNING , THREAD_READY ,
THREAD_BLOCKED , THREAD_DYING
3. ready_list contains lists of threads ready to run
4. all_list contains list of all threads
5. Each thread has time quanta T allocated to it after which it yield CPU to next
thread in the queue (checked by thread_tick)
6. When a thread is allocated CPU time, it is removed from ready queue and is
current thread, can be accessed by function thread_current()
7. BLOCKING and UNBLOCKING
a. If current thread is blocked by thread_block , the state of thread is changed
to THREAD_BLOCKED and next thread is scheduled by calling schedule()
b. When a thread is unblocked (using thread_unblock(thread) ) it is put back
to the ready_list and it’s state is changed to THREAD_READY.
1
8. When current_thread() yields CPU it is pushed back to end of ready_list and
schedule() is called.
9. schedule() calls next_thread_to_run() which pops the first item in ready_list and
returns to schedule() which then set this thread to current thread and calls
switch_threads() which does the context switch and then it calls
thread_schedule_tail() whose function is described above.
CONTEXT SWITCH - FUNCTIONS CALLED IN SEQUENCE
1. Context Switch when current thread time quanta has expired
1.1. thread_tick() is called by timer interrupt handler
1.2. thread_tick() calls intr_yield_on_return() which calls thread_yield()
1.3. thread_yield() push current thread at end of ready list and sets status of
current thread to ready and calls schedule()
1.4. schedule() calls next_thread_to_run() to get the next thread
1.5. schedule() calls swithc_threads(current thread, next thread) if next thread
!= current thread , switch_threads does the context switch
1.6. Then schedule() calls thread_schedule_tail() which activates page tables of
current thread and sets status of current thread to running , it destroys
previous thread if it’s state was THREAD_DYING , it also reset the time_slice
count of current running thread to 0
2. Context Switch when current thread is blocked
2.1. thread_block() set state of current thread to THREAD_BLOCKED and calls
schedule()
2.2. Same as 1.4 to 1.6
3. Context Switch when current thread has finished
3.1. thread_exit() sets status of current thread to THREAD_DYING and calls
schedule()
3.2. Same as 1.4 to 1.6
MULTILEVEL QUEUE IMPLEMENTATION
1. Files Modified
1.1. thread.h
1.2. thread.c
2. thread.h
2.1. In struct thread { …. } the following fields are added
2
2.1.1. Int8_t count;
//count = -1 initially for queue 2, count < 0 for queue 2
// count = 0 initially, count >= 0 for queue 1
2.1.2. Struct list_elem elem_low;
2.1.2.1. List element for queue B (ready_list_low)
3. thread.c
3.1. static struct list ready_list_low;
3.1.1. queue B
3.2. Static int clock_count = 0;
3.2.1. For printf statements
3.3. #define TIME_SLICE_LOW 8
3.3.1. # of timer ticks to give each thread of queue B.
3.4. #define WAITING_TIME 24
3.4.1. # of ticks queue B threads waits if not given CPU because of queue A
3.5. In function thread_init
3.5.1. list_init (&ready_list_low); //initialize ready_list_low aka queue B
3.6. In function thread_tick
3.6.1. The previous two line code - if(thread_ticks >= TIME_SLICE)
intr_yield_on_return() is replaced with
if(cur->count < 0){
//current thread executing belongs to queueB
if(list_empty(&ready_list)){
//their is nothing in queueA
if (thread_ticks >= TIME_SLICE_LOW){
printf("\n%d: thread %d goes running to L2 queue.\n", clock_count,
cur->tid);
intr_yield_on_return ();
}
}else{
3
//process in queueA has higher priority
printf("\n%d: thread %d goes running to L2 queue.\n", clock_count,
cur->tid);
intr_yield_on_return ();
}
}else{
//current thread executing belongs to queueA
if (thread_ticks >= TIME_SLICE){
if(cur->count >= 1){
cur->count = -1;
printf("\n%d: thread %d goes running to L2 queue.\n", clock_count,
cur->tid);
}else{
cur->count++;
printf("\n%d: thread %d goes running to L1 queue.\n", clock_count,
cur->tid);
}
intr_yield_on_return ();
}
}
3.7. In function thread_unblock
3.7.1. Instead of pushing thread directly to end of readly_list , thread is
pushed to list from which it was removed
4
if(t->count < 0){
list_push_back (&ready_list_low, &t->elem_low);
//printf("\n%d: thread %d goes from blocked to L2 queue.\n",
clock_count, t->tid);
}else{
list_push_back (&ready_list, &t->elem);
//printf("\n%d: thread %d goes from blocked to L1 queue.\n",
clock_count, t->tid);
}
3.8. Similarily in function thread_yield first it is cheked if thread should go to
ready_list or ready_list_low and then it is pushed at end of required queue
if (cur != idle_thread){
if(cur->count >= 0){
list_push_back (&ready_list, &cur->elem);
}else{
//printf("\n%d: thread %d goes running to L2 queue.\n",
clock_count, cur->tid);
list_push_back (&ready_list_low, &cur->elem_low);
}
}
5
3.9. In function next_thread_to_run() the thread is determined based on new
scheduling scheme
if (list_empty (&ready_list)){
if(list_empty(&ready_list_low)){
return idle_thread;
}else{
return list_entry (list_pop_front (&ready_list_low), struct thread,
elem_low);
}
}else{
return list_entry (list_pop_front (&ready_list), struct thread, elem);
}
The calling sequence of functions remains same as no new function is added , only
existing functions are modified.
