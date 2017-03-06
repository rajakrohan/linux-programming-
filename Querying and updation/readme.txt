There would be file containing a maximum of 100 student records. 
The record for eachstudent is kept in one line in the file and contains the following, separated by one or
more spaces: First_name (ascii string, one word with no spaces in between, max. 20
characters), Last_Name (ascii string, one word with no spaces in between, max 20
charaters), Roll_No (integer), CGPA (float). Roll no. is unique for a student, but
others may be the same for two students.

A process X, when started, first loads all the records in shared memory from a file
whose name is passed to it as a command line argument. It then goes to sleep, waking
up periodically (say every 5 seconds) to check if any of the data have been modified
(how will it know that?). If yes, it writes the entire content of the shared memory back
to the file. The process X runs forever. 

Any number of other processes can query and update the data by reading/writing the
shared memory. Specifically, a querying process Y is allowed to do the following
operations on the data:
• Search for a student record by roll no. (should show all data for that student)
• Update the CGPA of a particular student (the student is identified by the roll
no., which is unique for each student)
• Exit 
