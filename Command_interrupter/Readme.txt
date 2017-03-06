The shell will give a prompt for the user to type in a command, take the command, execute it,
and then give the prompt back for the next command (i.e., actually give the functionality of a
shell). Your program for the assignment should do the following:
o Give a prompt (like the $ prompt you get in Linux) for the user to type in a command.
The prompt should have the current working directory name (full path) followed by the
“>” sign (for ex., /usr/home/agupta/temp> ).
o Implement the following commands as builtin commands:
 cd <dir> : changes the directory to "dir"
 pwd : prints the current directory 
 mkdir <dir> : creates a directory called "dir"
 rmdir <dir> : removes the directory called "dir"
 ls : lists the files in the current directory. It should support ls both without any
option and with the option “-l”
 cp <file1> <file2>: copies the content of “file1” into “file2” only if the last
modification time of “file1” is more recent than that of “file2”. The filenames
may contain a full pathname. You can assume “file1” and “file2” are simple files
and not directories. No option of cp needs to be supported.
 exit : exits the shell
The commands are the same as the corresponding Linux commands by the same name.
Do "man" to see the descriptions. You can use the standard C library functions chdir,
getcwd, mkdir, rmdir, readdir, stat etc. to implement the calls.
All calls should handle errors properly, with an informative error message. For example,
cp will fail if the user calling it does not have read permission on “file1”; the call should
print a proper error message. Look up the perror call.
These commands are called builtin commands since your shell program will have a
function corresponding to each of these commands to execute them; no new process will
be created to execute them. (Note that all these commands are not builtin commands in
the bash shell, but we will make them so in our shell).
o Any other command typed at the prompt should be executed as if it is the name of an
executable file. For example, typing "./a.out" should execute the file a.out. The file
should be executed after creating a new process and then exec'ing the file onto it. The
parent process should wait for the file to finish execution and then go on to read the next
command from the user. The command typed can have any number of command line
arguments. 
