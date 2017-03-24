Two processes (not threads) A and B where A is a producer process and B is a
consumer process, with a queue of size 10 between them (you can borrow your code
from last assignments). Process A will act as the generator of requests, and process B
as the receiver of requests. For each request received, process B will create a thread to
handle that request, and then will come back to check if any other requests are there
in the queue. Process A and B runs perpetually, but with the following conditions: (i)
If process B is killed somehow (say ^C pressed by mistake), process A detect
that and also terminate automatically, and (ii) whenever a process terminates, it 
cleans up all shared memory, semaphore, thread mutex/condition variable etc. before it
terminates.
Process A does the following in an infinite loop: sleep for a random time between 0
and 2 seconds, generate a random number between -5 and +5, and add the number in
the queue. The number put in the queue acts as a request.
Each request contains just an integer x (positive or negative). Process B maintains a
global variable Ticket that is initialized to 100. For each request, B creates a thread
that executes a routine book_ticket() that takes the integer x in the request as a
parameter. If (Ticket – x) ≥ 0, book_ticket() sets Ticket = min(Ticket – x, 100), sleeps
for a random time between 0 and 2 seconds, prints the value of Ticket, and returns 1.
If (Ticket –x) < 0, Ticket is not changed, the function sleeps for a random time
between 0 and 2 seconds, prints a message saying request cannot be fulfilled, and
returns 0.
However, handling too many requests at once can slow down the server. So B also
keeps track how many requests are being serviced (threads started but not finished),
and if the number is greater than some fixed value (for this assignment, use 10),
