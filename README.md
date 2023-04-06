# Space Shell

This project is an implementation of a Linux shell using the C programming language for Operating Systems course. 

## Group members
*Lejla Muratović* \
*Emir Kugić*



## List of all files and directories
1. *shell.c*
2. *system_calls.c*
3.  */headers*:

- my_cmatrix.h
- my_df.h
- my_grep.h
- my_wc.h


## Outline
1. Custom prompt

2. The Space Shell can perform the following commands:
- *grep*
- *df*
- *wc* with flags:
    - '-l': _print new line count_ 
    - '-c': _print the byte count_
    - '-m': _print the character count_
    - '-w': _print the word count_
- *cmatrix* with flags:
    - '-r': _rainbow mode_
    - '-s': _space mode_
    - '-d': _donut mode_
    - '-B': _bold letters mode_

3. Piping
4. Redirecting
5. System calls:
- *fork()*
- *wait()*
- *exec()*
- *clone()*
- *execle()*
6. Implementation of a forkbomb



## Answers to task 1.5

### Q1: If we have a single-core, uniprocessor system that supports multiprogramming, how many processes can be in a running state in such a system, at any given time?

A1: In a single-core, uniprocessor system that supports multiprogramming, only one process can be in a running state at any given time. However, multiple processes can be in a ready or waiting state, awaiting their turn to be executed by the processor. The operating system uses scheduling algorithms to determine which process should be given access to the processor next, based on various criteria such as priority and waiting time.



### Q2: Explain why system calls are needed for a shared memory method of inter-process communication (IPC). If there are multiple threads in one process, are the system calls needed for sharing memory between those threads?


A2: System calls are necessary for a shared memory method of inter-process communication (IPC) because shared memory involves a region of memory that is accessible by multiple processes. Without the use of system calls, it would not be possible for different processes to access the same shared memory region simultaneously, which is a critical requirement for the shared memory IPC method.

When a process wants to access shared memory, it needs to call the appropriate system call provided by the operating system to establish a shared memory segment that can be accessed by other processes. This system call creates a shared memory segment, maps it to the address space of the calling process, and returns a pointer to the starting address of the shared memory segment. Other processes that want to access the same shared memory segment also need to call the same system call to map the shared memory segment to their own address spaces.

In the case of multiple threads within a single process, system calls are not needed for sharing memory between those threads. Threads within the same process share the same address space, and therefore can access the same memory locations without any need for system calls. In fact, sharing memory between threads within the same process is often a more efficient IPC method than using system calls for inter-process communication, because it avoids the overhead of system call invocation and context switching.






## How to compile and run

To use the Space Shell, simply open the terminal in the same directory as the program files and compile it with a compiler, like gcc, using the following command:

```bash
gcc -Wall -o shell shell.c
```
To start the program run the following command:
```bash
./shell
```


To compile the system_calls.c file:
```bash
gcc -D_GNU_SOURCE -o system_calls system_calls.c
```
To start the program run the following command:
```bash
./system_calls
```


## Challenges we encountered
- Debugging errors in piping
- Lack of resources
- Handling input/output redirection
- Handling signals and interrupts




## Sources
1. https://www.systutorials.com/how-to-get-the-hostname-of-the-node-in-c/
2. https://tuxthink.blogspot.com/2012/12/c-program-in-linux-to-find-username-of.html
3. https://brennan.io/2015/01/16/write-a-shell-in-c/
4. https://github.com/Sa3eedoo/Simple-shell-implementation/blob/main/shell.c
5. https://www.youtube.com/watch?v=rj7DOPPTkQA&t=340s&ab_channel=ServetGulnaroglu
6. http://www.dmulholl.com/lets-build/a-command-line-shell.html
7. https://www.geeksforgeeks.org/making-linux-shell-c/
8. https://stackoverflow.com/questions/11515399/implementing-shell-in-c-and-need-help-handling-input-output-redirection
9. https://codereview.stackexchange.com/questions/217099/linux-wc-command-in-c
10. https://www.youtube.com/watch?v=K8dCh3ZMLN8&t=175s&ab_channel=JacobSorber
11. https://forums.linuxmint.com/viewtopic.php?t=375974
12. http://www.vishalchovatiya.com/clone-system-call-example/