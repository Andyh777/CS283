1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

It looks for an EOF, recv() is used multiple times for the complete message to be transmissed

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Delimiters are used by a networked shell protocol along with null terminators. The commands might split incorrectly and create new commands

3. Describe the general differences between stateful and stateless protocols.

Stateful remember interactions while stateless only knows the current interaction

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP's advantages are that its faster than TCP because it doesn't check like TCP does.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Socket programming model