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

2. The Space Shell can perform all built-in commands, but it also calls the custom implementations of the following commands:
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


A2: System calls are required for a shared memory method of inter-process communication (IPC), it is because shared memory involves a part of memory that is accessible by multiple processes. Without using system calls, it wouldn't be possible for different processes to access the same shared memory region at the same time, which is necessary for the shared memory IPC method.

When a process wants to access shared memory, it has to use (call) an appropriate system call that is provided by the OS to establish a shared memory segment that later can be accessed by other processes. When a certain process calls a system call, it creates a space in memory that is shared between different parts of the program. This memory segment is mapped to the address space of the calling process, and the system call returns a pointer to the starting address of the shared memory. Other processes that want to access the created shared memory segment also need to call the same system call to map the shared memory to their own address spaces.

However, in case of multiple threads within a single process, system calls aren't needed for sharing memory between those threads. Threads within the same process share the same address space, and they can access the same memory locations without the use of system calls. It's actually often more efficient to share memory between threads within the same process instead of using system calls for inter-process communication. This is because using system calls involves additional overhead from invoking the system call and switching between different contexts, whereas sharing memory avoids these costs.






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
