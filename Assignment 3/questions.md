1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice because it is able to detect EOF and spaces between strings

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  I had tried allocating a fixed-size array and ran into the error of entering EOF, because the size had been predetermined, I would've had to trim the input of the trailing whitespace before I could parse anything.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming spaces is important because there can be spaces in between commands and we would need to store the command without the extra spaces in front and behind so the command can actually run.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  One example would be sending STDOUT to a file and then echoing that file, another would be directing STDIN from a file, a third way would be storing the STDOUT to a file by as a database. The challenges for these would be file permissions and proper file handling

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Piping takes the STDOUT of a command to use for another STDIN as a command while redirection deals with handling STDIN and STDOUT with files.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Having a seperate error output is important because it avoids confusion in output and error messages

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  If our commands fail, it should return a STDERR and a STDOUT of what the error was and a print message about what that error means. It can be merged by using >&