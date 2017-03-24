Takes two filenames, file1 and file2, as arguments from the command line 
Parent opens file1 for read and file2 for write (use open call) If file2 is not present it
will be created; if it is present it will be overwritten. If there is any error, exit with a
message (use perror).
o If no error, parent creates two pipes, pipe1 and pipe2.
o Parent creates a child process.
o Parent reads 100 bytes at a file from file1 and sends it to child process through write end
of pipe1.
o Parent then waits to receive an acknowledgement (the integer 0) or error (-1) from the
child in the read end of pipe2.
o Child, on receiving each 100 bytes, appends it to file2, and then sends 0 to the parent
through write end of pipe2 if the write is successful, and -1 if there is an error. If there is
any error, after sending the -1, the child prints an error message and exits. If the child
receives less than 100 bytes, it sends 0, prints “file copied successfully” and exits.
