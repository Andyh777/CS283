1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

I call waitpid on forked child process which ensures the parent waits for all child processes to complete. If I forgot to call waitpid, child processes would become into a zombie process

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2 because if left open, it can result in hanging endlessly because it only reads until EOF and can cause resource issues.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

CD changes the directory in a child process, which would result in no changes to the parent process when the program ends.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I could modify my implemenation by mallocing memory for pipes depending on many number of commands I have.
