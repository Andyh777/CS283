1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  fork allows for the program to keep running by making a copy. It allows for multiple tasks to run at the same time

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  It's unable to make a copy. I had a simple fork error message.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It looks inside the path enivorment variable directories.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Allows the child to finish, the parent process might not wait and the child will run in the wrong order

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It is the exit status of the child. It allows us to know if the issue lies in the child or parent

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: It handles quotes by ensuring it will be an single argument

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  Parsing white spaces with parsing the input. No issues

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are notifications that can be used to notify the programmer about events and are able the interrupt the rpocess

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL is when the process isn't responding and has to be force stopped. SIGTERM is when the process needs to close but finish all handling before closing. SIGINT is used to stop a running process.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  It is susepnded, it cannot be caught or ignored like SIGIN because it's treated as a pause until SIGCONT is given.
