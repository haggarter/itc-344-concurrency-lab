# Concurrency Lab

## Objective
Develop a basic parallel echo server that demonstrates handling multiple client connections concurrently. The focus will be on exploring two concurrency models: using threads and using processes. Students will also analyze potential race conditions, particularly in the threaded implementation.

## Scenario Overview
You are a new intern at EchoCo, a digital communications research company. The senior developer--your boss--has requested your help to upgrade an internal server to handle multiple client connections simultaneously. He has a few proof-of-concept implementations that he would like you to test out. He would normally try to spin up the programs himself, but he's taking his third vacation to Hawai'i this year, so he really can't be bothered.

The socket setup for accepting and handling client connections is pre-built and provided to you. Your task is to implement the logic for managing these connections, either by:

- Forking a new process for each client.
- Assigning each client connection to a thread in a thread pool.

## Learning Objectives
- Understand how to implement threading and process control in a C program
- Understand and implement concurrency using threads and processes
- Identify and address race conditions in a multi-threaded context
- Compare performance and resource usage between threading and forking models

## Getting Started
Read the section on [processes](#processes), on [threads](#threads), and on [their differences](#processes-vs-threads). Also check out the resources in [additional information](#additional-information).

## Instructions

### Part 1 - Process Server
Hey, you're the new intern, right? Sorry I'm not in to meet you face to face. I'll be boarding my flight soon, but I wanted to send some information over to you about our current project. EchoCo has long had a dedicated internal echo server for our researchers to help in their tests. Don't ask me why we need it; it is older than I am and the only reason we keep it around is the researchers love it. Hard to teach old dogs new tricks.

Anyway, R&D has been complaining recently that their employees are experiencing latency and connection issues with the server. It seemes we've outgrown the days when all we had were 3 researchers and they mostly just played Goldeneye in the breakroom. The server currently can only handle one request at a time, and it backs up quite a bit with their 32 researchers. We need to add concurrency to the server so that it can handle these loads.

The server was written in what everything was written in back in the day: C. I hear you're actually pretty well qualified to program in C, so I'm trusting this task to you. I came up with an idea to add concurrency by forking a new process for each client. Inelegant, I know, but it is rather simple to program and it would not require much refactoring on our part.

I have uploaded some starter files and some helper code to github. I need you to go through and flesh them out. Check out the process_server.c file. It should have comments on what you need to do to get a process up and running. Don't worry about any of the socket stuff and communicating with clients. I took care of that in help.c, and I've commented each place that you'll need to call methods from help.c.

Add the following to the file process_server.c
- main():
    In the while loop, call `accept_client(int sfd)` to wait for a new client to connect. This method will block until a client connects. As soon as a client has connected, call `fork()`. If it is the child process, call `handle_client(int sfd)`.
    
    *Note: After `handle_client(int sfd)` is finished, make sure you call `exit()`. Otherwise, the child process will continue through the program and will jump back to the while loop, effectively turning the child into a new server!*

### Testing Part 1
Once you have that program ready to roll, run the following command to compile it:
'''
gcc -o process_server process_server.c help.c
'''

Test that it acts as an echo server. Run the following to start it:
'''
./process_server 8080
'''

Then, run the following to start a telnet connection with the server:
'''
telnet localhost 8080
'''

Telnet should say that the connection was a success. Type anything you would like, followed by two new lines. You should see whatever you typed echoed back to you.

Once that is working, use the provided test_driver.py to verify that it can accept up to 32 concurrent clients. Run:
'''
python3 test_driver.py process_server
'''

### Part 2 - Signals

## Processes vs. Threads

### Processes
A process is an active instance of a program running on a computer. Each process has its own dedicated memory space, separate from other processes, ensuring they don’t interfere with one another. The operating system is responsible for managing processes, allocating the necessary resources—such as memory, CPU time, and file handles—for each one. The following diagram illustrates a single process in action.

> ![asingle process](https://github.com/user-attachments/assets/41dfca9c-dd91-45a2-97e6-95392ae44f8d)  

When only one process is running, the computer can focus all its resources on completing it efficiently. However, what happens when we need to run multiple instances of the same process or several different processes simultaneously? The diagram below shows how this scenario unfolds.

> ![multiple processes](https://github.com/user-attachments/assets/f8a9cbf0-3b39-415c-afec-d6853813999b)

This approach is not efficient and can overwhelm the CPU, causing processes to compete for resources and take longer to complete. To address these challenges, modern systems often rely on threads, which allow multiple tasks to run concurrently within a single process, reducing overhead and improving performance. Let’s explore how threads work and why they are a more efficient alternative in certain scenarios. 

### Threads

A thread is the smallest unit of execution within a process. While a process operates in its own isolated memory space, threads within the same process share memory and other resources like file descriptors. This shared environment makes threads lightweight and efficient, allowing for faster communication and reduced overhead compared to processes.

**How Threads Work** 
Threads run concurrently within a process, meaning they can execute tasks in parallel on multi-core processors or simulate parallelism on a single core through time-sharing. For example, a web browser might use threads to render a page, load images, and handle user input simultaneously, all within a single process. Using the same diagram as before, it would look something like this: 

![thread_in_process](https://github.com/user-attachments/assets/78b44cf4-e918-4387-8f2f-835d47987ad2)

**Why Threads are Beneficial**

- Efficiency: Since threads share the process's memory space, the overhead of creating and managing them is much lower than that of processes.
- Parallelism: Threads enable tasks to run simultaneously, making better use of multi-core processors and improving performance for multi-tasking.
- Communication: Because threads share memory, they can exchange information quickly and easily without needing complex Inter-Process Communication (IPC) mechanisms.

**Challenges with Threads**
While threads offer many advantages, they also introduce complexities, especially when multiple threads access shared resources:

 **Race Conditions:**
 A race condition occurs when two or more threads access shared data simultaneously, and the final outcome depends on the timing of their execution. For instance, if two threads increment the same counter variable without proper synchronization, they might overwrite each other's updates, leading to incorrect results.


**Synchronization and the Role of Semaphores:**
To prevent race conditions, synchronization mechanisms like semaphores are used. A semaphore acts as a signaling mechanism to control access to shared resources. For example, a semaphore can allow only one thread to modify a shared variable at a time, ensuring consistency. 

However, improper use of semaphores can lead to:

- Deadlocks: When two or more threads are waiting on each other to release resources, resulting in a standstill.
- Starvation: When a thread is perpetually delayed because higher-priority threads monopolize access to the semaphore.

Threads provide powerful tools for improving performance and resource utilization, but they require careful management to avoid these pitfalls. Let’s explore how these challenges are addressed in practice and why threads are a critical component of modern computing.

## Additional Information
Below are some sites that discuss more of the topics if you have any questions:  
[Processes vs. Threads](https://www.geeksforgeeks.org/difference-between-process-and-thread/#)  
[How semaphores work](https://www.geeksforgeeks.org/semaphores-in-process-synchronization/)

## Grade
- [ ] Successfully implement processes
- [ ] Successfully implement thread
- [ ] Identify the benefits and costs of each option
- [ ] Resolve race-case scenarios with threads
