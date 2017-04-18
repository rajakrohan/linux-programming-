Adding some basic
signal handling calls to Pintos.
Kept track of parent and children of a thread. The parent thread
of a thread X is the thread that called thread_create to create X. Added a
function called setlifetime(X) that will set the maximum lifetime of a process to X time
ticks. The default lifetime off a process (if not set by setlifetime()) is unlimited. See
below what the maximum lifetimes is used for.
We will support 5 signals with names SIG_CHLD, SIG_KILL, SIG_CPU and
SIG_UBLOCK, and SIG_USR. Each of them has a default handler (CHLD_hanlder,
KILL_handler, CPU_handler, UNBLOCK_handler, and USR_handler). The default
handler is called with two parameters which are the id of the thread that sent the signal.
The signals work as follows:
• SIG_CHLD is sent to a thread X by the OS when any of its children terminates. If
X has already terminated when one of its children dies, no signal is sent. The
handler for SIG_CHLD updates the count of children which has died so far and
prints both the total number of children created by X (till the signal is delivered)
and the number of children still alive.
• SIG_KILL can be sent by one thread to another. The handler simply terminates
the receiving thread and prints a message. However, this signal can only be
delivered by a thread to its direct children, otherwise the signal is not sent.
• SIG_CPU signal is sent to a thread by the OS if a predefined maximum lifetime
of a thread is exceeded. The lifetime of a thread includes both its running and
waiting times. The handler prints the maximum lifetime set and terminates the
thread.
• SIG_UNBLOCK signal can be sent by any thread to any other thread. The
handler unblocks the receiving thread if it is blocked. If the receiving thread is
already unblocked, no action is taken.
• SIG_USR can be sent by any thread to any other thread. The default handler just
prints which thread sent the signal to which thread and returns.
With the above signals, implemented the following functions similar to
signals in Linux, except that process ids will be replaced by thread ids.
• signal(): since we do not support user-defined signal handlers, the only options
supported for the second argument are SIG_IGN and SIG_DFL with same
meaning. SIG_KILL cannot be ignored, so calling signal(SIG_KILL, SIG_IGN)
returns success but does nothing.
• kill() (should be valid only for SIG_KILL, SIG_UNBLOCK, and SIG_USR
subject to the conditions stated, all other cases are error cases. Returns 0 on
success, -1 on error)
• sigemptyset(), sigfillset(), sigaddset(), sigdelset(), sigprocmask() (all of them
have the same meaning as in Linux, with the same meaning of masking signals.
The mask is 4 LSB bits of an unsigned short. Masking SIG_KILL has no effect.).
For sigprocmask(),implemented all three SIG_BLOCK,
SIG_UNBLOCK, and SIG_SETMASK options for the first three parameters.
Signals can be raised at any time (subject to the conditions mentioned in the description
of the signals). All signals other than SIG_UNBLOCK are delivered to a thread when the
thread is about to run again (next context switch where the thread is chosen for running).
SIG_UNBLOCK signal is delivered to the receiving thread when the next context switch
happens irrespective of which thread is chosen to run.
All signals are queued till delivery. Multiple same signals to the same thread are queued
up as a single signal and delivered to the receiving thread only once (with the last thread
id taken as the sending thread. Note that this does not make good sense for SIG_CHLD
signal, but we will do this like this for simplicity for now, ignoring SIG_CHLD can
anyway mess up the count).